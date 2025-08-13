# ESP32_audio_whisper

This project demonstrates a complete pipeline for real-time audio acquisition, digitization, wireless transmission, and AI-based speech transcription using the ESP32 microcontroller, MAX4466 analog microphone, Whisper model, and GPT-4o.

## 🎯 Overview

The system was developed as part of a bachelor thesis and integrates embedded audio processing with modern AI tools. This gadget-like prototype has proven to be a practical and efficient tool when paired with lecture recordings, meetings, and similar scenarios. Its portability and low cost make it easy to build, maintain, and deploy in a variety of settings.
### 🔧 Core Functionality

1. **Audio capture** via MAX4466 analog microphone  
2. **ADC conversion** on ESP32 using I2S + DMA in "ADC-only" mode  
3. **TCP transmission** to a local Python server  
4. **Saving audio** as `.wav` file  
5. **Transcription** using OpenAI's Whisper model (`large-v3-turbo`)  
6. **Correction & summarization** using GPT-4o via OpenAI API


### System architecture 

<img width="1325" height="696" alt="image" src="https://github.com/user-attachments/assets/0a858136-200e-4ec0-a72a-cb8630fcc496" />



## ▶️ How to Run

### ESP32 (Firmware)
1. Install [PlatformIO](https://platformio.org/) in VSCode.  
2. Open the `src/` directory
3. Configure your Wi-Fi credentials in `wifi.c`
4. Upload the firmware:  
   ```
   pio run -t upload -t monitor
   ```

### Python Server
1. Install dependencies:
   ```bash
   pip install openai-whisper openai
   ```
2. Set your OpenAI API keys in audio_server.py and audio_ai.py
3. Run the server:
   ```bash
   python server/audio_server.py
   ```

## 📦 Models Used

- **Whisper**: `large-v3-turbo` (optimized for speed and memory usage)
- Croatian transcription benchmark WER is around 11% which ranks it among most accurate transcription tools in this language
- <img width="651" height="941" alt="image" src="https://github.com/user-attachments/assets/0c27a23a-862d-4355-88ee-97e1aac86c0c" />

- **GPT-4o**: Used for transcript reconstruction and summarization  

## 📌 Notes

- The ESP32 microcontroler uses I2S in ADC mode and streams audio in chunks of 8192 bytes
- Output files include:
  - `.wav` – raw audio
  - `.txt` – transcript
  - `.summary.txt` – language-polished transcript + summary


## 📝 License

This project is licensed under the [MIT License](LICENSE).
