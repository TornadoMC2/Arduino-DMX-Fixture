# Arduino DMX Fixture

Arduino DMX Fixture is an open-source project that leverages the Arduino platform to control DMX-compatible lighting fixtures. The project aims to provide an easy-to-use, flexible solution for custom lighting control applications using the DMX512 protocol.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Overview
The Arduino DMX Fixture project allows hobbyists and professionals alike to create custom lighting controllers using affordable Arduino hardware. With support for the DMX512 protocol, this project is perfect for stage lighting, architectural installations, or any other application that requires precise control over multiple lighting fixtures.

## Features
- **DMX512 Support:** Control up to 512 channels per DMX universe.
- **Customizable Fixture Programming:** Easily define lighting behaviors and responses.
- **Real-Time Control:** Adjust lighting parameters on the fly.
- **Modular Design:** Expandable architecture that allows additional features and integrations.
- **Open-Source:** Community-driven development with contributions welcomed.

## Hardware Requirements
- **Arduino Board:** Any Arduino compatible board (Uno, Mega, etc.) that meets the project’s I/O requirements.
- **DMX Transceiver/Shield:** A DMX shield or transceiver module (e.g., MAX485-based) to interface with DMX equipment.
- **Power Supply:** Appropriate power supply to drive the Arduino and DMX hardware.
- **Cabling and Connectors:** DMX cables and connectors as required by your fixture setup.

## Software Requirements
- **Arduino IDE:** Version 1.8 or higher.
- **DMX Library:** Install the [DMXSerial](https://github.com/mathertel/DMXSerial) library or another compatible DMX library for Arduino.
- **Additional Libraries:** Any additional libraries listed in the `requirements.txt` or documentation specific to your hardware configuration.

## Usage
After uploading the firmware:
- **Wiring:** Connect the DMX transceiver/shield to your Arduino and ensure proper wiring to the DMX fixtures.
- **Configuration:** Adjust the DMX address and channel settings in the code as needed for your specific setup.
- **Operation:** Once powered, the Arduino will begin sending DMX signals according to the programmed behavior. You can modify lighting effects by editing the source code and re-uploading as necessary.

## Code Structure
- **`src/`**: Contains the main Arduino sketch and supporting source files.
- **`lib/`**: Contains third-party libraries or custom modules used by the project.
- **`docs/`**: Documentation files and configuration examples.
- **`examples/`**: Example configurations and use cases for common fixture setups.

## Contributing
Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Commit your changes with clear messages.
4. Submit a pull request with a detailed description of your changes.

For major changes, please open an issue first to discuss what you would like to change.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements
- Thanks to the Arduino community for continuous support and inspiration.
- Special thanks to contributors who have helped enhance the DMX functionality and expand the project’s capabilities.
