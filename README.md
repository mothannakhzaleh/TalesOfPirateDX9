
# TalesOfPirateDX9

**About This Repository**
**This repository contains the source code for _Tales of Pirate_, updated to DirectX 9. Over the past few years, we collaborated with several freelancers to modernize the engine from its original DX8 foundation.**

**In response to certain groups within the community profiting from leaked versions of this code, we have decided to make the source public for the benefit of all developers. Please note that we do not offer support for future bugs or technical issues.**

While the core DX9 upgrade is functional, it originated from a separate development branch; you may encounter minor inconsistencies. This remains an excellent starting point for new projects. Finally, we are not responsible for any legacy bugs or exploits present in the original code, as these were inherited from previous owners.

### **Credits & Copyright**

**Ownership & Contributions** All rights to the engine upgrades and source modifications belong to **[SatisfyTeam](https://satisfy.live/)**, **Mothannkh**, and the private benefactors who funded the project’s development but chose to remain anonymous.

**Terms of Use** This source is provided to the community for development purposes. While we encourage innovation, we ask that users respect the work put in by the team and the contributors who made this progress possible.

### CURRENT SOURCE SPECIFICATIONS

**Core Engine & Networking**

-   Upgraded from DirectX 8 to DirectX 9
    
-   Client and Server built with VS 2022 (C++14)
    
-   TopNet source integration
    
-   RSA/AES connection encryption (Client/Server)
    
-   Encrypted .clu files
    
-   Core packets updated to use Smart Pointers
    
-   Anti-WPE and Anti-DDoS protection
    
-   Anti-Dupe system
    

**Gameplay Systems**

-   Offline Stalls and Offline Mode
    
-   Full Mount System
    
-   Guild Bank with transaction logs
    
-   AuthorizedGM system
    
-   Reworked Jackpot system
    
-   In-game Shop remade (Classic style, 9 items per page)
    

**User Interface & Features**

-   Multi-resolution support
    
-   60 FPS toggle option
    
-   HP and SP bars displayable as values or percentages
    
-   World Map with Zoom In/Out
    
-   Drop Info with integrated Drop Filter
    
-   Right-click inventory menu options
    
-   Friend/Enemy combat modes
    
-   Visibility toggles for Effects, Apps, and Mounts
    

**Scripting & Structure**

-   Restructured LUA folders for better organization
    
-   Balanced Vanilla LUA setup
    
-   Simplified file structure with intuitive variable and function names
    
-   Modular design for easy code migration (copy/paste friendly)
vs config file  **[Here]([https://satisfy.live/](https://drive.google.com/file/d/1GR8GdSNe-UocUNpkvChoHBaqZJaTcuuS/view))**
