<div align="center">
  <img src="Vanished.png" alt="vanished.myself" width="200" style="border-radius:10px;"/>
  <br>
  <h1>vanishing.persona.</h1>
  <p><strong>A local, privacy-first automation tool to bulk delete posts, tweets, and messages across multiple platforms.</strong></p>
</div>

---

## 🎯 Purpose
`vanishing.persona` is designed to efficiently streamline deleting messages or media, covering your tracks on all social media channels you use. Manually clearing interactions is tedious—this application does the heavy lifting entirely **offline**. 

*Note: Only client-side logs or messages you have direct right to delete will be removed. We cannot scrub server-side end-to-end encrypted backup systems if you do not have deletion access.*

## 🔒 Privacy-First
Data deletion must be handled locally. We enforce **no cloud storage API keys** and **no telemetry/external tracking**:
- Keys are encrypted locally using Windows BCrypt (AES-GCM).
- Operates totally autonomously—giving you direct agency over your data footprint.

---

## 🏗️ Development Roadmap

> The current state is foundational scaffolding. We are building the core infrastructure before platform integrations.

### 1. Core Architecture
- [x] UI Engine: **Sciter** for lean, fast interface rendering.
- [x] Secure Local Storage: Encrypted JSON for API credentials.
- [x] Thread-safe Rate-Limited Job Queue: Executes safe, automated data deletion tasks cleanly. 

### 2. Platform Integrations *(Coming Soon)*
- [ ] Implement local-only OAuth flows
- [ ] Twitter / X API (delete posts, un-like tweets)
- [ ] Reddit API (delete comments and threads)
- [ ] Discord Integration (wipe server messages and DMs)

### 3. UI / User Experience
- [ ] Interactive Dashboard for Account Configurations
- [ ] Post/Message Filters (date range, keyword scanning, engagement thresholds)
- [ ] Real-time Progress Tracking for running deletion queues

### 4. Security & Release
- [ ] Local Security Vulnerability Testing (guarantee zero trace footprint)
- [ ] User self-serve API Key Generation Guides
- [ ] Standalone OS Executables (Windows, macOS, Linux)

---

## ❓ FAQ

### Does this store any private sessions (cookies, tokens, auth, keys)?
**Absolutely not.** We guarantee that this tool contains zero manipulation or shadow software collecting your info. Check the source code yourself. Keys are natively encrypted with zero-trust storage methodologies. 

### Where can I get support?
For problems, bug reports, or assistance, please open a GitHub issue or contact the project maintainers directly via repository discussions.
