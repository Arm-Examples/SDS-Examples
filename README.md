[![License](https://img.shields.io/github/license/Arm-Examples/SDS-Examples?label)](https://github.com/Arm-Examples/SDS-Examples/blob/main/LICENSE)
[![Build_AC6](https://img.shields.io/github/actions/workflow/status/Arm-Examples/SDS-Examples/Build_AC6.yml?logo=arm&logoColor=0091bd&label=Build%20examples%20with%20AC6)](https://github.com/Arm-Examples/SDS-Examples/tree/main/.github/workflows/Build_AC6.yml)
[![Test_Alif_E7](https://img.shields.io/github/actions/workflow/status/Arm-Examples/SDS-Examples/Test_Alif_E7.yml?logo=arm&logoColor=0091bd&label=Test%20Alif%20AppKit-E7%20example)](https://github.com/Arm-Examples/SDS-Examples/tree/main/.github/workflows/Test_Alif_E7.yml)
[![Test_Alif_E8](https://img.shields.io/github/actions/workflow/status/Arm-Examples/SDS-Examples/Test_Alif_E8.yml?logo=arm&logoColor=0091bd&label=Test%20Alif%20DevKit-E8%20example)](https://github.com/Arm-Examples/SDS-Examples/tree/main/.github/workflows/Test_Alif_E8.yml)
[![Test_ST_KeywordSpotting](https://img.shields.io/github/actions/workflow/status/Arm-Examples/SDS-Examples/Test_ST_KeywordSpotting.yml?logo=arm&logoColor=0091bd&label=Test%20ST%20Keyword%20Spotting)](https://github.com/Arm-Examples/SDS-Examples/tree/main/.github/workflows/Test_ST_KeywordSpotting.yml)
[![Test_ST_MotionRecognition](https://img.shields.io/github/actions/workflow/status/Arm-Examples/SDS-Examples/Test_ST_MotionRecognition.yml?logo=arm&logoColor=0091bd&label=Test%20ST%20Motion%20Recognition)](https://github.com/Arm-Examples/SDS-Examples/tree/main/.github/workflows/Test_ST_MotionRecognition.yml)

# SDS-Examples

[<img src="./images/SDS_FW_Thumbnail.png" alt="SDS-Framework: Workbench for AI model and DSP development" width="318" height="202" align="left">](https://armkeil.blob.core.windows.net/developer/Files/videos/KeilStudio/DevOps_With_Keil_MDK_Webinar.mp4#t=11:27 "SDS-Framework: Workbench for AI model and DSP development")

This repository contains examples that show the usage of the [Synchronous Data Streaming (SDS) Framework](https://github.com/ARM-software/SDS-Framework).

**[Watch this video](https://armkeil.blob.core.windows.net/developer/Files/videos/KeilStudio/DevOps_With_Keil_MDK_Webinar.mp4#t=11:27 "SDS-Framework: Workbench for AI model and DSP development")** and explore the projects below.

<br clear="left"/>

## Quick Start

1. Install [Keil Studio](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack) and [Arm SDS](https://marketplace.visualstudio.com/items?itemName=Arm.cmsis-sds) from the
   VS Code marketplace.
2. Clone this Git repository into a VS Code workspace.
3. Open the [CMSIS View](https://mdk-packs.github.io/vscode-cmsis-solution-docs/userinterface.html#2-main-area-of-the-cmsis-view)
   in VS Code and use the ... menu to choose an example via *Select Active Solution from workspace*.
4. The related tools and software packs are downloaded and installed. Review progress with *View - Output - CMSIS Solution*.
5. In the CMSIS view, use the
   [Action buttons](https://github.com/Open-CMSIS-Pack/vscode-cmsis-solution?taq=readme#action-buttons) to build,
   load and debug the example on target hardware.
6. Follow the instructions in the example README and use the SDS view to show, record, and playback data streams.

## Example Description

The SDS examples are configured for various Evaluation Boards and use the [MDK-Middleware](https://www.keil.arm.com/packs/mdk-middleware-keil/overview/) for the [SDSIO Interface](https://arm-software.github.io/SDS-Framework/main/sdsio.html). New hardware targets can be added using board layers that provide the required API interfaces.
The examples are configured for [Keil Studio for VS Code](https://www.keil.arm.com/).
Run a Blinky example for the related board first to verify tool installation.

| Example                                                | Description   |
|---                                                         |---            |
| [Alif/AppKit-E7_USB](./Alif/AppKit-E7_USB/README.md)                   | SDS connecting via USB to [Alif AppKit-E7 board](https://www.keil.arm.com/boards/alif-semiconductor-appkit-e7-aiml-d1-34b5d51/guide/). |
| [Alif/DevKit-E8_ETH](./Alif/DevKit-E8_ETH/README.md)                   | SDS connecting via Ethernet to [Alif DevKit-E8 board](https://www.keil.arm.com/boards/alif-semiconductor-devkit-e8-a1-c8b9599/features/). |
| [ST/B-U585I-IOT02A/MotionRecognition](./ST/B-U585I-IOT02A/MotionRecognition/README.md) | SDS connecting via USB to  [STMicroelectronics B-U585I-IOT02A board](https://www.keil.arm.com/boards/stmicroelectronics-b-u585i-iot02a-revc-c3bc599/features/). Implements motion recognition algorithm with sensor interface.  |
| [ST/B-U585I-IOT02A/KeywordSpotting](./ST/B-U585I-IOT02A/KeywordSpotting/README.md)     | SDS connection via USB to [STMicroelectronics B-U585I-IOT02A board](https://www.keil.arm.com/boards/stmicroelectronics-b-u585i-iot02a-revc-c3bc599/features/). Implements keyword spotting algorithm with audio interface.  |
| [SDS-STM32N6](https://github.com/arm-examples/SDS-STM32N6)              | SDS connecting via USB to [STM32N6570-DK board](https://www.keil.arm.com/boards/stmicroelectronics-stm32n6570-dk-revc-f2017e0/guide/) |

> [!TIP]
> Each example is self-contained in a directory. The tool configuration and CI workflows are in separate directories listed below.

| More                                                 | Description   |
|---                                                         |---            |
| [Alif/AppKit-E7_USB](https://github.com/arm-examples/SDS-STM32N6)                   | SDS connecting via USB to [Alif AppKit-E7 board](https://www.keil.arm.com/boards/alif-semiconductor-appkit-e7-aiml-d1-34b5d51/guide/). |
| [Alif/DevKit-E8_ETH](./Alif/DevKit-E8_ETH/README.md)                   | SDS connecting via Ethernet to [Alif DevKit-E8 board](https://www.keil.arm.com/boards/alif-semiconductor-devkit-e8-a1-c8b9599/features/). |
| [ST/B-U585I-IOT02A/MotionRecognition](./ST/B-U585I-IOT02A/MotionRecognition/README.md) | SDS connecting via USB to  [STMicroelectronics B-U585I-IOT02A board](https://www.keil.arm.com/boards/stmicroelectronics-b-u585i-iot02a-revc-c3bc599/features/). Implements motion recognition algorithm with sensor interface.  |
| [ST/B-U585I-IOT02A/KeywordSpotting](./ST/B-U585I-IOT02A/KeywordSpotting/README.md)     | SDS connection via USB to [STMicroelectronics B-U585I-IOT02A board](https://www.keil.arm.com/boards/stmicroelectronics-b-u585i-iot02a-revc-c3bc599/features/). Implements keyword spotting algorithm with audio interface.  |

## Tool and CI Directory Structure

| File/Directory                            | Content |
|---                                        |--- |
| [vcpkg-configuration.json](vcpkg-configuration.json) | Setup for development tools on desktop. |
| [.ci](./.ci)                              | Files that are related to the Continuous Integration (CI) tests. |
| [.github/workflows](./.github/workflows)  | GitHub Actions scripts for build and execution tests. |

<!--
## Webinar

The following webinar shows how to use the SDS framework and the examples in this repository:

[![Watch the video](SDS_preview.png)](https://armkeil.blob.core.windows.net/developer/Files/videos/KeilStudio/20250916_SDS_Webinar.mp4)
-->

## Continuous Integration (CI)

The repository uses [GitHub Actions](.github/workflows) to test project build with AC6 and execute algorithm tests.
Refer to [Understanding GitHub Actions](https://docs.github.com/en/actions/get-started/understand-github-actions) and [Arm FVPs](https://arm-software.github.io/AVH/main/infrastructure/html/avh_gh_actions.html) documentation for more information.

| <div style="width:150px"> CI Workflow </div>                                   | Description |
|---                                                                             |---  |
| [Build_AC6](./.github/workflows/Build_AC6.yml)                                 | Use Arm Compiler for Embedded (AC6) to create binaries for different configuration of targets, build types, and boards. |
| [Test_Alif_E7](./.github/workflows/Test_Alif_E7.yml)                           | Build the binary for an object detection algorithm and execute a regression test using an FVP model. Compare the original SDS recording with the newly generated recording produced during the simulator run. Store the build outputs and SDS recordings as workflow artifacts.  |
| [Test_Alif_E8](./.github/workflows/Test_Alif_E8.yml)                           | Build the binary for an object detection algorithm and execute a regression test using an FVP model. Compare the original SDS recording with the newly generated recording produced during the simulator run. Store the build outputs and SDS recordings as workflow artifacts.  |
| [Test_ST_KeywordSpotting](./.github/workflows/Test_ST_KeywordSpotting.yml)     | Build the binary for a keyword spotting algorithm and execute a regression test using an FVP model. Compare the original SDS recording with the newly generated recording produced during the simulator run. Store the build outputs and SDS recordings as workflow artifacts.   |
| [Test_ST_MotionRecognition](./.github/workflows/Test_ST_MotionRecognition.yml) | Build the binary for a motion recognition algorithm and execute a regression test using an FVP model. Compare the original SDS recording with the newly generated recording produced during the simulator run. Store the build outputs and SDS recordings as workflow artifacts. |

## Related

- Raise questions or issues for these examples on the repository [github.com/ARM-software/SDS-Framework](https://github.com/ARM-software/SDS-Framework/tree/main?tab=readme-ov-file#issues-and-labels).
- [Arm Examples - Edge AI and Machine Learning](https://github.com/Arm-Examples#edge-ai-and-machine-learning) contains several other examples that use SDS.
- [SDS Framework - Documentation](https://arm-software.github.io/SDS-Framework/main/index.html).
