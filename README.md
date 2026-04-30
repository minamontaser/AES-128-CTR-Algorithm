# AES-128-CTR from 101 (C++)

A zero-dependency C++ implementation of AES-128-CTR. It explores the Rijndael cipher through manual $GF(2^8)$ arithmetic and NIST transformations. Designed for educational deep dives, it allows studying the internal mechanics of symmetric encryption without the abstraction of external libraries like OpenSSL.

---

## 🚀 Project Overview

This repository provides a clean C++ implementation of the **AES-128** block cipher operating in **CTR mode**. By encrypting successive values of a counter, this implementation transforms a block cipher into a stream cipher, allowing for parallelization and eliminating the need for padding.

*   **Zero Dependencies:** Pure C++ with no external crypto libraries.
*   **Educational:** Clear separation between helper utilities and core AES transformations.
*   **CTR Mode:** Practical demonstration of nonce management and counter logic.

---

## 📂 File Structure

*   **`AES-128-CTR.h`**: The engine. Contains the `AESCTR` class and all cryptographic logic.
*   **`main.cpp`**: The driver. A CLI-based tool for encryption and decryption tasks.

---

## 🛠️ Class Architecture

### **Public Interface**
The public section is kept minimal for ease of use:
*   `encrypt(string plaintext, string key)`: Returns a pair of vectors containing the random nonce and the ciphertext.
*   `decrypt(vector<uint8_t> cipher, vector<uint8_t> key, vector<uint8_t> nonce)`: Returns the original plaintext.

### **Private Members**

#### 🔹 Helper Functions
Utility logic for data conversion and math:
*   `stringToBytes()`, `bytesToHex()`, `hexToBytes()`
*   `sbox()`: S-Box lookup table.
*   `xorBlocks()`: Bitwise XOR for state and counter.
*   `xtimes()`, `multiplyGF28()`: Finite field arithmetic in $GF(2^8)$.
*   `rotWord()`, `subWord()`: Key schedule primitives.
*   `generateNonce()`, `buildNonceCounter()`, `increament_ctr()`: CTR management.

#### 🔹 Core Functions
Standard NIST AES transformations:
*   `addRoundKey()`, `getRoundKey()`
*   `subBytes()`
*   `shiftRows()`
*   `mixColumns()`
*   `keyExpansion()`: Key schedule derivation.
*   `AES_main_block()`: Primary 10-round AES logic.
*   `AES_CTR_private()`: Orchestration of the CTR mode logic.

---
