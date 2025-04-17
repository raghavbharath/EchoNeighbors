from fastapi import FastAPI
from pydantic import BaseModel
from fastapi.middleware.cors import CORSMiddleware
import requests

app = FastAPI()
stored_tokens = {}  # For testing only. Use a DB for production.

# CORS for frontend → backend communication
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Open to all in dev
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Correct spelling!
class TokenPayload(BaseModel):
    token: str
    user: str

# Save token from frontend
@app.post("/register-token")
async def register_token(data: TokenPayload):
    stored_tokens[data.user] = data.token  # ✅ Actually saves it now
    print(f"✅ Token saved for {data.user}: {data.token}")
    return {"message": "Token received"}

# Manually trigger push notification to all saved users
@app.post("/trigger-intruder")
async def trigger_intruder():
    if not stored_tokens:
        return {"message": "❌ No tokens saved"}
    for user, token in stored_tokens.items():
        print(f"📢 Triggering push for {user}...")
        send_push(token, f"🚨 Intruder detected near {user}'s house!")
    return {"message": "Push sent"}

# Expo push function
def send_push(token, message):
    payload = {
        "to": token,
        "sound": "default",
        "title": "🚨 Intruder Alert!",
        "body": message
    }
    res = requests.post("https://exp.host/--/api/v2/push/send", json=payload)
    print("📤 Push sent:", res.status_code, res.text)