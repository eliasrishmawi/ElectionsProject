# Election Events Simulator

This project was developed for the **CS240: Data Structures** course (Winter Semester 2024–2025).  
It simulates the process of **Greek elections** by processing a sequence of events that create districts, parties, stations, candidates, voters, and ultimately form the parliament.

---

## 📌 Project Overview
- **5 Parties**
- **56 Districts**
- Events define the flow of the election:
  - Creating districts, stations, parties, candidates, and voters
  - Casting and counting votes
  - Forming government
  - Creating the final parliament

---

## ⚡ Supported Events
- `A` – Announce elections (initialize structures)  
- `D <did> <seats>` – Create district  
- `S <sid> <did>` – Create station  
- `P <pid>` – Create party  
- `C <cid> <did> <pid>` – Register candidate  
- `R <vid> <did> <sid>` – Register voter  
- `U <vid>` – Unregister voter  
- `E` – Delete empty stations  
- `V <vid> <sid> <cid>` – Cast vote  
- `M <did>` – Count votes for district  
- `G` – Form government (distribute leftover seats)  
- `N` – Form parliament  
- `I <did>` – Print district 
- `J <sid> <did>` – Print station
- `K <pid>` – Print Party 
- `L` – Print Parliament 

---

## ⚙️ How to Build & Run
### Compile
make
### Run
./cs240Elections.exe testfiles/<file_name>
there are 3 test files: large, medium and small.

# Author
Elias Rishmawi
