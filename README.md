# APPS-R2D — Accelerator Pedal Position Sensor Safety System

> **Safety-critical firmware for Formula Student electric vehicle** | STM32 NUCLEO-F303K8 · Embedded C · KiCad

---

## Overview

APPS-R2D (Accelerator Pedal Position Sensor – Ready to Drive) is a dual-redundant throttle safety system built for Team Fateh's Formula Student electric vehicle. It monitors two independent throttle position sensors and one brake pressure sensor in real time, detecting implausibility conditions and blocking unintended torque commands to prevent drive-by-wire hazards.

Designed and validated to comply with **FSAE/Formula Bharat EV safety regulations**.

---

## The Problem

In a drive-by-wire electric vehicle, a single faulty sensor reading can command full throttle while the driver is braking — a potentially fatal condition. FSAE rules mandate that any implausibility between dual APPS sensors lasting more than 100ms must cut motor torque immediately.

This system solves that.

---

## Key Features

- **Dual-redundant APPS sensing** — Two independent throttle position sensors cross-checked in real time
- **Brake Plausibility Check (BPC)** — Detects simultaneous brake + throttle input and cuts torque
- **<10ms interrupt response** — Safety interlocks trigger within one control loop cycle
- **Implausibility detection** — Flags >10% deviation between the two APPS sensors
- **Fault-safe design** — System defaults to zero torque on any sensor fault or implausibility condition
- **KiCad schematic** — Full dual-sensor circuit with voltage dividers and protection

---

## Hardware

| Component | Details |
|-----------|---------|
| MCU | STM32 NUCLEO-F303K8 |
| Throttle Sensors | Opkon linear potentiometer |
| Brake Sensor | Analog pressure sensor (0–5V) |
| IDE | STM32CubeIDE |
| PCB Design | KiCad |
| Power | 3.3V regulated via NUCLEO onboard |

---

## System Architecture

```
TPS_1 (ADC CH1) ──┐
                   ├──► Implausibility Check ──► Motor Controller Output
TPS_2 (ADC CH2) ──┘          │
                              │ >10% deviation OR
BPS   (ADC CH3) ──────────────┘ brake + throttle simultaneously
                              │
                              ▼
                        TORQUE CUT (0 PWM)
```

---

## Firmware Logic

```c
// Core safety check — runs every control loop cycle (~1ms)
void apps(void){
	 diff = fabs(tps1 - tps2);
    tps1 = ((float)tps1_raw * 3.3f) / 4095.0f;
	       tps2=((float)tps2_raw*3.3f)/4095.0f;

           percent1=(tps1/3.3)*100;
           percent2=(tps2/2.5)*100;

	 diff=fabsf(percent1-percent2);

	  if ((tps1 == tps2) || diff>10 || tps1 ==0.0|| tps2 ==0.0) {
			if ((current_millis - previous_millis) >= 100) {
			if(r2d == GPIO_PIN_RESET || r2d==GPIO_PIN_SET){
			HAL_GPIO_WritePin(GPIOB,MC_ENABLE, GPIO_PIN_RESET);
			previous_millis = current_millis;
		//    		            		HAL_Delay(500);
			}
		}
			}
			else {
				if ((r2d == GPIO_PIN_SET)){
					HAL_GPIO_WritePin(GPIOB, MC_ENABLE, GPIO_PIN_SET);
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);

					}
			}

}
```

---

## Performance

| Metric | Result |
|--------|--------|
| Safety interlock response time | **< 10ms** |
| APPS implausibility detection | **< 1ms** (per loop cycle) |
| Torque cut latency after 100ms fault | **Compliant with FSAE T.4.2.5** |
| Dual-sensor deviation threshold | **10%** |

---

## Project Structure

```
APPS-R2D/
├── Core/
│   ├── Src/
│   │   ├── main.c              # Entry point, peripheral init, Implausibility + BPC logic
│   │  
│   └── Inc/
│       ├── main.h
├── KiCad/
│   ├── APPS_R2D.kicad_sch      # Full circuit schematic
│   └── APPS_R2D.kicad_pcb      # PCB layout
├── Docs/
│   └── APPS_R2D_Validation.pdf # Test results + calibration log
└── README.md
```

---

## Setup & Flashing

```bash
# Clone the repo
git clone https://github.com/Vedant0527/APPS-R2D.git

# Open in STM32CubeIDE
# File → Import → Existing Projects into Workspace

# Build: Project → Build All (Ctrl+B)
# Flash: Run → Debug (F11) or use ST-LINK
```

**Requirements:**
- STM32CubeIDE v1.13+
- STM32 HAL drivers (auto-included via CubeIDE)
- ST-LINK V2 programmer

---

## Safety Compliance

This system is designed to meet the following FSAE/Formula Bharat EV rules:

- **T.4.2.5** — APPS Implausibility detection and torque cut
- **T.4.3.3** — Brake System Plausibility Device (BSPD) requirements
- **EV.4.6** — Torque encoder plausibility check

---

## Team

**[Vedant Shri Agarwal](https://linkedin.com/in/vedant-shri-agarwal)** — Firmware & PCB Design  
Team Fateh Formula Racing · Thapar University

---

## License

MIT License — see [LICENSE](LICENSE) for details.
