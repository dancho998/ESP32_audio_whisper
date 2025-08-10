# ESP32_audio_whisper

This project demonstrates a complete pipeline for real-time audio acquisition, digitization, wireless transmission, and AI-based speech transcription using the ESP32 microcontroller, MAX4466 analog microphone, Whisper model, and GPT-4o.

## 🎯 Overview

The system was developed as part of a bachelor thesis and integrates embedded audio processing with modern AI tools. It provides a local and privacy-focused alternative to cloud-based speech recognition systems.

### 🔧 Core Functionality

1. **Audio capture** via MAX4466 analog microphone  
2. **ADC conversion** on ESP32 using I2S + DMA in "ADC-only" mode  
3. **TCP transmission** to a local Python server  
4. **Saving audio** as `.wav` file  
5. **Transcription** using OpenAI's Whisper model (`large-v3-turbo`)  
6. **Correction & summarization** using GPT-4o via OpenAI API  

## 📂 Folder Layout
graphql
Copy
Edit
src/
├── main.c              # ESP32 firmware entry point
├── mic.c / mic.h       # Microphone input handling (I2S + DMA)
├── wifi.c / wifi.h     # Wi-Fi and TCP client code
├── audio_server.py     # TCP server to receive audio
├── audio_ai.py         # AI processing (Whisper + GPT-4o)
├── audio_1.wav         # Example recorded audio
├── audio_1.txt         # Example raw transcription
└── audio_1.summary.txt # Example cleaned & summarized transcript

CMakeLists.txt          # Build system for ESP-IDF / PlatformIO
LICENSE
README.md
Note: Example .wav, .txt, and .summary.txt files are included so you can see the output format without running the full pipeline.

## ▶️ How to Run

### ESP32 (Firmware)
1. Install [PlatformIO](https://platformio.org/) in VS Code.  
2. Open the `src/` directory.  
3. Configure your Wi-Fi credentials in `wifi.c`.  
4. Upload the firmware:  
   ```
   pio run -t upload -t monitor
   ```

### Python Server
1. Install dependencies:
   ```bash
   pip install openai-whisper openai
   ```
2. Set your OpenAI API keys in audio_server.py and audio_ai.py.  
3. Run the server:
   ```bash
   python server/audio_server.py
   ```

## 📦 Models Used

- **Whisper**: `large-v3-turbo` (optimized for speed and memory usage)  
- **GPT-4o**: Used for transcript post-processing and summarization  

## 📌 Notes

- The ESP32 firmware uses I2S in ADC mode and streams audio in chunks of 8192 bytes.  
- The Python server auto-generates output filenames and handles data if data stream stops.  
- Output files include:
  - `.wav` – raw audio
  - `.txt` – transcript
  - `.summary.txt` – language-polished summary

## ⚠️ Security

**Do not commit `.wav`, `.txt`, or `.env` files.**  
This is enforced via `.gitignore`.

## 📝 License

This project is licensed under the [MIT License](LICENSE).
