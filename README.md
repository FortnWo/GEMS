# 智能农业大棚环境监测与控制系统

> 基于 STM32L431RCT6 的物联网农业大棚监测与控制系统，集成多传感器采集、本地 OLED 显示、异常报警、自动化设备控制与云端数据上报。

## 📖 项目简介

本项目是一个面向农业大棚场景的嵌入式物联网应用，运行在 STM32L431RCT6 微控制器上。系统通过多种传感器实时采集大棚内的环境数据（空气温湿度、土壤温度、光照强度等），在 OLED 屏幕本地轮播显示，并基于设定的安全阈值进行异常检测与自动化设备控制（通风、喷淋、遮阳、补光、加热）。同时通过 WiFi 模组连接 ThingsCloud 云平台，实现数据上报与远程指令下发控制。

### ✨ 核心功能

- 🌡️ **多传感器环境采集**：空气温湿度、土壤温度、光照强度、NTC 热敏电阻测温
- 🖥️ **OLED 本地显示**：开机启动界面 + 温度/湿度/光照多页轮播
- 🚨 **异常检测与报警**：基于上下限阈值的异常状态判断，蜂鸣器报警
- ⚙️ **自动化设备控制**：通风器、喷淋器、遮阳板、补光灯、加热器联动控制
- ☁️ **云平台互联互通**：MQTT 协议连接 ThingsCloud，数据上报与远程下发
- 📋 **任务调度系统**：基于时间片的轻量级任务调度框架
- 🔘 **按键交互**：状态机实现的短按/长按/双击/三击检测

## 🛠️ 硬件平台

| 组件 | 说明 |
| --- | --- |
| 主控芯片 | STM32L431RCT6 (ARM Cortex-M4, 80MHz) |
| 温湿度传感器 | AHT10（I2C，地址 0x70） |
| 光照传感器 | BH1750（I2C，地址 0x46） |
| 土壤温度传感器 | DS18B20（单总线） |
| 空气温度传感器 | NTC 3950 热敏电阻（ADC 采样） |
| 显示屏 | 0.96 寸 OLED（128×64，I2C） |
| 蜂鸣器 | GPIO 控制有源蜂鸣器 |
| WiFi 模组 | ESP-01S / 兼容 AT 指令模组（LPUART1） |
| 调试串口 | USART1（printf 重定向） |
| 受控设备 | 通风器 / 喷淋器 / 遮阳板 / 补光灯 / 加热器 |

### 引脚映射（节选）

| 外设 | 引脚 | 端口 |
| --- | --- | --- |
| KEY 按键 | PIN_13 | GPIOC |
| DS18B20 数据线 | PIN_1 | GPIOA |
| LED_RED | PIN_8 | GPIOC |
| BEEP 蜂鸣器 | PIN_8 | GPIOA |
| 设备控制 1 (Dev_Ctrl1) | PIN_11 | GPIOA |
| Dev_Ctrl1A15 | PIN_15 | GPIOA |

## 📂 项目结构

> ⚠️ `Debug/`（构建输出目录）、生成的 Makefile（`makefile`、`objects.mk`、`sources.mk`、`subdir.mk` 等）、编译产物（`*.o`、`*.elf`、`*.map`、`*.d` 等）以及编辑器/系统临时文件均已通过 `.gitignore` 忽略，不纳入版本管理。克隆仓库后由 STM32CubeIDE 自动生成，无需手动提交。

```
GEMS/                          # 工程根目录
├── .gitignore                 # Git 忽略规则（构建产物、编辑器/系统临时文件等）
├── .project                   # CubeIDE 工程描述文件
├── .cproject                  # CubeIDE C/C++ 构建配置
├── .mxproject                 # CubeMX 工程元数据
├── .settings/                 # CubeIDE 工作区设置
├── STM32_Ebed.ioc             # STM32CubeMX 工程配置
├── STM32L431RCTX_FLASH.ld     # 链接脚本
├── STM32_Ebed Debug.launch    # 调试启动配置
├── 原理图.pdf                 # 硬件原理图
├── README.md                  # 项目说明文档
├── Core/
│   ├── Inc/                   # 头文件
│   │   ├── main.h             # 主程序头文件（引脚定义、模块汇总）
│   │   ├── controller.h       # 异常控制逻辑与阈值结构
│   │   ├── alarm.h            # 报警结构与状态
│   │   ├── taskschedule.h     # 时间片任务调度
│   │   ├── thingcloud.h       # 云平台通信与 WiFi 数据结构
│   │   ├── interface.h        # OLED 界面显示
│   │   ├── AHT10.h / BH1750.h # 温湿度 / 光照驱动
│   │   ├── DS18B20.h / ntc_3950.h  # 土壤温 / NTC 驱动
│   │   ├── oled.h / font.h    # OLED 显示与字库
│   │   ├── key.h / led.h / Beep.h
│   │   ├── adc.h / dma.h / i2c.h / tim.h / usart.h / gpio.h
│   │   └── delay.h / timeslice.h / customize.h
│   ├── Src/                   # 源文件
│   │   ├── main.c             # 程序入口与外设初始化
│   │   ├── controller.c       # 异常扫描与设备控制决策
│   │   ├── alarm.c            # 报警执行逻辑
│   │   ├── taskschedule.c     # 任务组件表与调度器
│   │   ├── thingcloud.c       # WiFi/MQTT 数据处理与上传
│   │   ├── interface.c        # OLED 多页界面
│   │   ├── AHT10.c / BH1750.c # 传感器驱动实现
│   │   ├── DS18B20.c / ntc_3950.c
│   │   ├── oled.c / font.c
│   │   ├── key.c / led.c / Beep.c
│   │   ├── adc.c / dma.c / i2c.c / tim.c / usart.c / gpio.c
│   │   ├── stm32l4xx_hal_msp.c # HAL MSP 初始化
│   │   ├── stm32l4xx_it.c     # 中断服务函数
│   │   ├── system_stm32l4xx.c # 系统时钟配置
│   │   └── syscalls.c / sysmem.c
│   └── Startup/
│       └── startup_stm32l431rctx.s  # 启动文件
└── Drivers/                   # HAL 库与 CMSIS
    ├── STM32L4xx_HAL_Driver/
    └── CMSIS/
```

## 🧩 软件架构

### 任务调度框架

系统采用基于时间片的轻量级任务调度器（`taskschedule.c`），由 TIM16 定时中断驱动 `TaskScheduleCb()` 自减计数，主循环调用 `TaskHandle()` 执行到期任务：

| 任务 | 周期 | 功能 |
| --- | --- | --- |
| `LedTask` | 50 ms | LED PWM 呼吸灯效果 |
| `Data_Sensor` | 1000 ms | 采集 NTC / DS18B20 / AHT10 / BH1750 |
| `Page_Show` | 3400 ms | OLED 多页轮播显示 |
| `DataUpLoad` | 10000 ms | 上报传感器数据与设备状态到云平台 |
| `ExceptionAlarm` | 1000 ms | 异常报警检测与蜂鸣器控制 |
| `ExceptionHandle` | 1800000 ms (30 min) | 异常状态持续检测与设备联动 |

### 异常控制策略

`controller.c` 实现了基于阈值的闭环控制：

- **安全阈值**：每个环境参数配置 `Low`/`High` 上下限（见 `controller.h`）
- **状态判定**：`NORMAL` / `HIGH` / `LOW` 三态枚举
- **持续检测**：异常需持续 `CHECK_TIME_MIN`（30 min）才会触发控制动作，避免误触发
- **设备联动**：
  - 温度过高 → 开启通风器，关闭加热器
  - 温度过低 → 开启加热器，关闭通风器
  - 其他参数（湿度、光照、土壤温湿度、CO₂）→ 对应设备联动

### 云平台通信

通过 LPUART1 与 WiFi 模组以 AT 指令交互，使用 MQTT 协议接入 ThingsCloud：

- `ThingCloud_Conn()`：配置 MQTT 用户名/密码并建立连接、订阅
- `DataUpLoad()`：定时上报 `temperature`、`humidity`、`luminosity`、`soil_temp`、设备状态等属性
- `WifiAPPDataHandle()`：解析云平台下发的 `attributes/push` 与 `command/send` 指令，控制设备开关

> ⚠️ 使用前请在 `thingcloud.c` 中替换为你自己的 ThingsCloud `userName` 与 `userPwd`：
> ```c
> char userName[]="sxogjo1rkdhgc54j";
> char userPwd[]="teYOgi0rcZ";
> ```

## 🚀 快速开始

### 环境要求

- **IDE**：STM32CubeIDE 1.x+
- **固件库**：STM32L4xx HAL Driver（已随工程附带）
- **调试器**：ST-Link V2 / J-Link
- **WiFi 模组**：ESP-01S 或兼容 AT-MQTT 指令的模组

### 编译与烧录

1. 使用 STM32CubeIDE 打开 `STM32_Ebed.ioc` 或工程根目录
2. 配置 WiFi 模组接入网络与 ThingsCloud 凭证（`thingcloud.c`）
3. 点击 **Build** 编译工程
4. 连接 ST-Link，点击 **Debug / Run** 烧录至目标板

### 运行流程

```
上电 → HAL_Init → SystemClock_Config → 外设初始化 (GPIO/DMA/USART/TIM/ADC/I2C)
     → 传感器初始化 (DS18B20 / OLED / AHT10 / BH1750)
     → StartupInterface() 显示启动界面
     → 启动 PWM / DMA-ADC / 定时器 / DMA-UART 接收
     → ThingCloud_Conn() 连接云平台
     → 主循环：KeyHandle() + TaskHandle()
```

## 🔧 关键模块说明

| 模块 | 文件 | 说明 |
| --- | --- | --- |
| 主程序 | `main.c` | 初始化与主循环 |
| 异常控制 | `controller.c` / `controller.h` | 阈值判定与设备决策 |
| 报警 | `alarm.c` / `alarm.h` | 异常蜂鸣报警 |
| 任务调度 | `taskschedule.c` / `timeslice.h` | 时间片调度器 |
| 云通信 | `thingcloud.c` / `thingcloud.h` | MQTT 上报与下发 |
| 界面 | `interface.c` / `interface.h` | OLED 多页显示 |
| AHT10 驱动 | `AHT10.c` / `AHT10.h` | 温湿度采集 |
| BH1750 驱动 | `BH1750.c` / `BH1750.h` | 光照度采集 |
| DS18B20 驱动 | `DS18B20.c` / `DS18B20.h` | 单总线土壤温度 |
| NTC 驱动 | `ntc_3950.c` / `ntc_3950.h` | ADC 热敏电阻测温 |
| OLED | `oled.c` / `oled.h` / `font.c` | 图形与文字显示 |
| 按键 | `key.c` / `key.h` | 状态机按键检测 |

## ⚙️ 配置参数

可在对应头文件中调整以下关键参数：

| 参数 | 位置 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `CHECK_TIME_MIN` | `controller.h` | 30 | 异常持续触发控制的最短时间 (min) |
| `S_TIME` | `alarm.h` | 180 | 报警生效延迟时间 (min) |
| `lowLimit` / `highLimit` | `controller.c` | 见源码 | 各环境参数上下限阈值 |
| `taskCycle` | `customize.c` | 30 | 自定义任务周期 |
| `DMA_REC_SIZE` | `thingcloud.h` | 1024 | WiFi DMA 接收缓冲大小 |

## 📝 备注

- 本工程基于 STM32CubeMX 生成，用户代码集中在 `/* USER CODE BEGIN */` 与 `/* USER CODE END */` 之间，重新生成代码时不会被覆盖
- 部分传感器（如 CO₂、土壤湿度）当前为占位数值，可按需接入实际传感器
- 云平台凭证请在部署前替换为自有账号
- `Debug/` 目录及编译过程中生成的文件（`*.o`、`*.elf`、`*.map`、`*.d`、生成的 Makefile 等）已被 `.gitignore` 忽略，不会提交到版本库；首次用 STM32CubeIDE 打开工程并编译后会自动生成

## 📜 许可证

- 应用层用户代码：由项目作者所有
- STM32 HAL Driver / CMSIS：遵循 STMicroelectronics 的 LICENSE（见 `Drivers/` 下 `LICENSE.txt`）
