import whisper
import openai

class AudioAI:
    def __init__(self, whisper_model="large-v3-turbo", device="cpu", openai_key=None, gpt_model="gpt-3.5-turbo"):
        self.model = whisper.load_model(whisper_model, device=device)
        self.gpt_model = gpt_model
        
        self.openai_client = openai.OpenAI(
            api_key=openai_key
        )

    def transcribe(self, filename, language="hr"):
        result = self.model.transcribe(filename, fp16=False, language=language)
        return result["text"]

    def summarize(self, transcript, language="hr"):
        prompt = (f"Napravi sažetak sljedećeg transkripta na jeziku {language}, "
                  "jasno napiši o čemu se razgovara i koji su glavni zaključci:\n\n"
                  f"{transcript}")
        response = self.openai_client.chat.completions.create(
            model=self.gpt_model,
            messages=[
                {"role": "system", "content": f"Piši sažetke na jeziku {language}, budi koncizan i jasan."},
                {"role": "user", "content": prompt}
            ],
            temperature=0.3,
            max_tokens=700,
        )
        return response.choices[0].message.content.strip()