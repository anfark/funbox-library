# FunBox Library

Die **FunBox Library** enthält die Hardware-Abstraktion und das Game-Framework für die FunBox-Konsole.

Ein Beispielprojekt mit Spielen findest du im **FunBox Workshop**:

https://github.com/anfark/funbox-workshop

## Voraussetzungen

Für die Entwicklung wird die **Arduino IDE 2** mit ESP32-Unterstützung benötigt.

## 1. ESP32-Unterstützung installieren

Öffne in der Arduino IDE:

**File → Preferences → Additional Boards Manager URLs**

und füge folgende URL hinzu:

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

Öffne anschließend den **Boards Manager**, suche nach:

```text
esp32
```

und installiere **esp32 by Espressif Systems**.

Für die FunBox wird ein ESP32-C3 verwendet. Wähle unter **Tools → Board**:

```text
ESP32C3 Dev Module
```

Aktiviere anschließend:

```text
Tools → USB CDC On Boot → Enabled
```

Wähle danach unter **Tools → Port** den angeschlossenen ESP32-C3 aus.

## 2. Dependencies installieren

Die FunBox verwendet folgende Arduino-Libraries:

* **Audio Tools**
  https://github.com/pschatzmann/arduino-audio-tools
* **Adafruit NeoMatrix**
  https://github.com/adafruit/Adafruit_NeoMatrix
* **Adafruit NeoPixel**
  https://github.com/adafruit/Adafruit_NeoPixel
* **Adafruit GFX Library**
  https://github.com/adafruit/Adafruit-GFX-Library
* **Adafruit SSD1306**
  https://github.com/adafruit/Adafruit_SSD1306

### Audio Tools

**Audio Tools muss manuell installiert werden.**

Öffne das GitHub-Repository:

https://github.com/pschatzmann/arduino-audio-tools

Wähle dort:

**Code → Download ZIP**

und anschließend in der Arduino IDE:

**Sketch → Include Library → Add .ZIP Library...**

Wähle die heruntergeladene ZIP-Datei aus.

### Andere Dependencies

Die übrigen benötigten Libraries sollten von der Arduino IDE **automatisch als Dependencies der FunBox Library installiert werden**.

Du musst sie normalerweise nicht einzeln installieren.

Falls beim Kompilieren trotzdem eine Library fehlt, kannst du sie entweder über den **Library Manager** installieren oder das entsprechende GitHub-Repository als ZIP herunterladen und über

**Sketch → Include Library → Add .ZIP Library...**

manuell hinzufügen.

## 3. FunBox Library installieren

Lade die FunBox Library als ZIP herunter.

Öffne anschließend in der Arduino IDE:

**Sketch → Include Library → Add .ZIP Library...**

und wähle die heruntergeladene ZIP-Datei aus.

Danach kann die Library verwendet werden:

```cpp
#include <FunBox.h>

FunBox box;

void setup() {
  box.setup();
}

void loop() {
  box.update();
}
```

## Spiele registrieren

Spiele werden über ihre Game Definition bei der FunBox registriert:

```cpp
void setup() {
  box.addGame<SnakeGame>();

  box.setup();
}
```

Weitere Beispiele findest du im **FunBox Workshop**:

https://github.com/anfark/funbox-workshop
