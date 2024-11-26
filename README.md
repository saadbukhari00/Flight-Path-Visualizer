# Vehicle-to-Cloud Communication Simulation

## Overview

This repository simulates a vulnerable vehicle-to-cloud communication system, highlighting various cybersecurity issues. The system demonstrates common vulnerabilities in cloud communication, device pairing, data encryption, firmware updating, and the potential for data breaches.

The project also simulates different types of security threats, such as weak authentication, unencrypted data transmission, and improper encryption handling.

## Modules

### 1. **Cloud Communication (`cloud_communication.py`)**
   - **Purpose**: Handles communication between the vehicle and the cloud, with optional encryption.
   - **Key Features**:
     - `enable_encryption()`: Enables encryption for cloud communication.
     - `send_data()`: Sends data to the cloud, with or without encryption.
     - `simulate_data_breach()`: Simulates a cloud data breach.

### 2. **Logger (`logger.py`)**
   - **Purpose**: Handles logging for various system activities.
   - **Key Features**:
     - Logs messages with different severity levels (INFO, WARNING, ERROR).

### 3. **Controller (`controller.py`)**
   - **Purpose**: Manages the vehicle registration and data transmission.
   - **Key Features**:
     - `authenticate()`: Authenticates the vehicle based on a weak code (simulating vulnerability).
     - `receive_unencrypted_data()`: Receives unencrypted data.
     - `receive_encrypted_data()`: Decrypts and processes encrypted data (with error handling).

### 4. **Encryption (`encryption.py`)**
   - **Purpose**: Provides symmetric encryption and decryption for sensitive data.
   - **Key Features**:
     - `encrypt()`: Encrypts the data using AES encryption.
     - `decrypt()`: Decrypts the encrypted data, with error handling for decryption failures.

### 5. **Vehicle (`vehicle.py`)**
   - **Purpose**: Simulates a vehicle that communicates with the cloud and the controller.
   - **Key Features**:
     - `pair_device()`: Simulates the device pairing process with a weak code.
     - `send_unencrypted_data()`: Sends data without encryption.
     - `send_encrypted_data()`: Sends encrypted data.
     - `update_firmware()`: Simulates firmware updating.

### 6. **Main (`main.py`)**
   - **Purpose**: Coordinates the interactions between the vehicle, controller, and cloud communication modules.
   - **Key Features**:
     - Simulates vehicle pairing, data transmission, firmware update, and cloud communication.

## Simulation Workflow

1. **Vehicle Pairing**: A weak vehicle pairing code is used for initial device authentication.
2. **Data Transmission**:
   - **Unencrypted Data**: Sends data without encryption, demonstrating a security risk.
   - **Encrypted Data**: Encrypts data using AES encryption, but improper decryption handling results in errors.
3. **Firmware Update**: The vehicle performs a firmware update to mitigate vulnerabilities.
4. **Cloud Communication**:
   - Sends data to the cloud with optional encryption.
   - Simulates a potential data breach in the cloud.

## Vulnerabilities Identified

1. **Weak Pairing Code**: The vehicle device pairing process uses a weak code (less than 4 characters), making it susceptible to unauthorized access.
   - **Mitigation**: Implement multi-factor authentication or stronger codes for pairing.

2. **Unencrypted Data Transmission**: Data is sent without encryption, exposing it to potential eavesdropping.
   - **Mitigation**: Ensure that all data is encrypted during transmission using strong encryption algorithms.

3. **Improper Decryption Handling**: The decryption process does not properly handle errors, leading to failure when the data is corrupted or tampered with.
   - **Mitigation**: Implement better error handling and validation for decryption operations.

4. **Outdated Firmware**: The vehicle uses outdated firmware, which may have known vulnerabilities.
   - **Mitigation**: Ensure regular updates and patch management to keep the firmware up-to-date.

5. **Potential Data Breach**: The cloud communication module is susceptible to data breaches, exposing sensitive vehicle data.
   - **Mitigation**: Enable encryption for all cloud communications and implement strong access control mechanisms.

## Omissions / Lack of Mitigations

- **Lack of Secure Communication Protocols**: The code does not include secure communication protocols such as HTTPS or secure WebSockets for cloud communication. Using these protocols would help secure data during transmission.
- **No Integrity Checks**: The system does not perform integrity checks (e.g., hash-based message authentication codes) on transmitted data, which could detect tampering during transmission.
- **Limited Authentication Mechanisms**: The vehicle device pairing process uses a weak authentication method (a simple code), which leaves the system vulnerable to unauthorized access.

## How to Run

1. Install the required dependencies:
   ```bash
   pip install pycryptodome
   ```

2. Run the simulation:
   ```bash
   python main.py
   ```

## Conclusion

This simulation serves as a demonstration of various vulnerabilities and security flaws present in vehicle-to-cloud communication systems. By understanding and addressing these vulnerabilities, better security practices can be implemented to protect against potential threats.
   
---

## References

- AES Encryption: [PyCryptodome](https://www.pycryptodome.org/)
- Logger Configuration: [Python Logging Module](https://docs.python.org/3/library/logging.html)

