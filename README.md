# projectSI

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    Proiectul este conceput pentru persoanele care dețin un autoturism. Își propune să rezolve probleme proprietarilor de mașini care din diferite considerente își găsesc mașina zgâriată în parcare. Acesta este reprezentat de un sistem de monitorizare a mașinii dintr-o parcare. În cazul în care mașina unei persoane este atinsă de un autovehicul, proprietarul va fi notificat prin aplicația mobilă și va avea salvate pe un card SD imagini cu mașina de care i-a fost zgâriat vehiculul. 



Modul de functionare: 


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    La prima alimentare a sistemului, camera Huskylens este pornită pentru a încărca modelul învățat care se află pe cardul SD și pentru setarea modului în care dorim ca acesta să funcționeze, adică funcționalitate de object tracking. După realizarea acestui setup, camera este stinsă. 


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    Senzorul ultrasonic calculează distanțele către obiectele din apropiere. În cazul în care este detectat un obiect la o distanță mai mica de 30 cm, se consideră că acesta este aproape și se pornește camera Huskylens. 


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    În continuare timp de 3 minute de la apropierea obiectului, camera scanează după obiecte de tipul modelului învățat anterior. 


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    Dacă este detectat un obiect, în situația noastră - o mașină, se urmărește starea citită de la senzorul de vibrații timp de 1 minut de la detecția obiectului. În cazul în care au fost detectate vibrații, camera Huskylens face poze și le salvează pe card, iar proprietarul va fi notificat pe telefon.




Testarea sistemului:


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    Până la testarea întregului sistem unitar, am început prin a testa fiecare subsistem al sistemului. În general, câte o componentă pe rând.


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    Am testat funcționarea corectă a componentelor utilizate:


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;      - placa Arduino Uno


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;      - Senzorului de vibrații sw-420


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;      - Senzorului ultrasonic hc-sr04


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;      - Camera Huskylens


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;      - Modulul bluetooth hc-05

    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     După testarea fiecărei componente, am început integrarea pe rând a lor, adăugând câte o componentă o dată. Astfel, am verificat funcționarea progresivă a sistemului.


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     Pentru testarea întregului sistemului, ne-am folosit de 2 mașini la scară mică pentru a imita scenariul real.




Scenariul de funcționare:


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     - se alimentează sistemul, se așteaptă pornirea camerei Huskylens, scrierea algoritmului și încărcarea modelului de pe cardul SD (acțiuni ale căror stări se pot observa și pe Serial Monitor în Arduino IDE). Se așteaptă stingerea camerei;

    
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    - ulterior, în Serial Monitor va fi afișată distanța detectată de senzorul ultrasonic. La apropierea unui obiect de senzor la mai puțin de 30 cm, se observă aprinderea camerei;
 
    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - camera detectează obiectul în cazul în care acesta respectă modelul învățat, moment în care, timp de 1 minut, se citește starea senzorului de vibrații (toate informațiile sunt afișate în Serial Monitor);
 
       
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     - dacă senzorul detectează vibrație (mașina a fost zgâriată), camera va face poze și le salvează pe cardul SD, iar cu ajutorul modulului bluetooth se trimite notificare pe telefon;


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     - pozele de pe card pot fi observate citind cardul pe calculator, iar notificarea va apărea pe ecranul telefonului.


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     Aplicația de telefon a fost creată prin intermediul aplicației web Mit App Inventor.
