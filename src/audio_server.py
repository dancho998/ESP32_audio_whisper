import socket
import wave
import time
from audio_ai import AudioAI

SAMPLE_RATE = 43000
CHANNELS = 1
SAMPLE_WIDTH = 2
PORT = 12345
counter = 1

API_KEY = "api_key"  # your api key here

ai = AudioAI(
    whisper_model="large-v3-turbo",
    device="cpu",
    openai_key=API_KEY
)

print(f"Listening for ESP32 audio stream on port {PORT}...")

srv = socket.socket()
srv.bind(("0.0.0.0", PORT))
srv.listen(1)

while True:
    conn, addr = srv.accept()
    conn.settimeout(5.0)
    filename = f"audio_{counter}.wav"
    counter += 1
    print(f"Connected by {addr}. Saving to {filename}.")
    wav = wave.open(filename, "wb")
    wav.setnchannels(CHANNELS)
    wav.setsampwidth(SAMPLE_WIDTH)
    wav.setframerate(SAMPLE_RATE)
    count = 0
    t0 = time.time()
    try:
        while True:
            try:
                data = conn.recv(8192)
                if not data:
                    break
                count += len(data)
                wav.writeframesraw(data)
                if time.time() - t0 > 1:
                    print("Bytes/sec:", count, "Implied sample rate:", count // 2)
                    count = 0
                    t0 = time.time()
            except socket.timeout:
                continue
    except KeyboardInterrupt:
        print("Stopping server...")
        break
    finally:
        conn.close()
        wav.close()
        print(f"Saved as {filename}")

        try:
            print("Transcribing with Whisper...")
            transcript = ai.transcribe(filename, language="hr")
            with open(filename.replace('.wav', '.txt'), 'w', encoding='utf-8') as f:
                f.write(transcript)

            print("Summarizing via OpenAI GPT API...")
            summary = ai.summarize(transcript, language="hr")
            with open(filename.replace('.wav', '.summary.txt'), 'w', encoding='utf-8') as f:
                f.write(summary)

            print("--- SUMMARY ---\n" + summary + "\n--- END ---")
        except Exception as e:
            print(f"AI processing error: {e}")
        print("Waiting for next connection (or ^C to quit)...")