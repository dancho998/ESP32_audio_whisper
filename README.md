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

## 📁 Project Structure

```
ESP32_audio_whisper/
├── firmware/                  # ESP32 code (PlatformIO or ESP-IDF)
│   ├── main.c
│   ├── wifi.c / wifi.h
│   ├── mic.c / mic.h
│   └── platformio.ini
│
├── server/                    # Python server & AI processing module
│   ├── audio_server.py
│   └── audio_ai.py
│
├── output/                    # Audio and text output (ignored via .gitignore)
│   ├── audio_1.wav
│   ├── audio_1.txt
│   └── audio_1.summary.txt
│
├── README.md
├── LICENSE
└── .gitignore
```

## ▶️ How to Run

### ESP32 (Firmware)
1. Install [PlatformIO](https://platformio.org/) in VS Code.  
2. Open the `firmware/` directory.  
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
2. Set your OpenAI API key (recommended via `.env`).  
3. Run the server:
   ```bash
   python server/audio_server.py
   ```

## 📦 Models Used

- **Whisper**: `large-v3-turbo` (optimized for speed and memory usage)  
- **GPT-4o**: Used for transcript post-processing and summarization  

## 📌 Notes

- The ESP32 firmware uses I2S in ADC mode and streams audio in chunks of 8192 bytes.  
- The Python server auto-generates output filenames and handles timeout if data stream stops.  
- Output files include:
  - `.wav` – raw audio
  - `.txt` – transcript
  - `.summary.txt` – language-polished summary

## ⚠️ Security

**Do not commit `.wav`, `.txt`, or `.env` files.**  
This is enforced via `.gitignore`.

## 📝 License

This project is licensed under the [MIT License](LICENSE).
