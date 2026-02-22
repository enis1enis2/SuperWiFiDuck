# Super WiFi Duck

<p align="center">
<img alt="WiFi Duck Logo" src="img/thumbnail.jpg" width="640">
</p>

Bu proje, WiFi Duck tabanını ESP32 S2/S3 çiplerinin yerel USB desteğiyle birleştirir.  
Böylece ek donanım gerektirmeden USB HID (klavye) emülasyonu ile WiFi üzerinden yönetilebilir bir BadUSB aracı elde edersiniz.

ESP32 S2/S3 aynı anda birden fazla USB cihazı gibi davranabilir.  
İleride USB fare ve USB disk özellikleri de eklenebilir.

Kurulum ve yükleme adımları orijinal WiFi Duck projesinden farklıdır; aşağıdaki bölümleri takip edin.

Video Demo:
<p align="center">
<img alt="WiFi Duck Demo" src="img/demo.gif" width="320">
</p>

---

- [Super WiFi Duck](#super-wifi-duck)
  - [Proje Hakkında](#proje-hakkında)
  - [Kullanım](#kullanım)
  - [ESP32 S2/S3'e Yükleme](#esp32-s2s3e-yükleme)
  - [Script Yazımı](#script-yazımı)
    - [Temel Mantık](#temel-mantık)
    - [Fonksiyonlar](#fonksiyonlar)
    - [Standart Tuşlar](#standart-tuşlar)
    - [Değiştirici Tuşlar](#değiştirici-tuşlar)
    - [Diğer Tuşlar](#diğer-tuşlar)
    - [Numpad Tuşları](#numpad-tuşları)
    - [Örnek](#örnek)
  - [CLI Komutları](#cli-komutları)
    - [Genel](#genel)
    - [SPIFFS Dosya Yönetimi](#spiffs-dosya-yönetimi)
  - [Hata Ayıklama](#hata-ayıklama)
  - [Geliştirme](#geliştirme)
    - [Web Dosyalarını Düzenleme](#web-dosyalarını-düzenleme)
    - [Klavye Tanımlayıcısını Değiştirme](#klavye-tanımlayıcısını-değiştirme)
    - [Klavye Düzeni (Locale) Çevirisi](#klavye-düzeni-locale-çevirisi)
  - [Sorumluluk Reddi](#sorumluluk-reddi)
  - [Lisans](#lisans)
  - [Teşekkürler](#teşekkürler)

## Proje Hakkında

WiFi Duck, tuş enjeksiyonu saldırıları ve BadUSB kavramını öğrenmek için kullanıcı dostu bir açık kaynak projedir.

USB klavye emülasyonu ile bu tür araçlar, USB portu olan sistemlerde çok kısa sürede giriş simülasyonu yapabilir.  
Bunun nedeni, işletim sistemlerinin klavyeleri güvenilir giriş aygıtı olarak kabul etmesidir.

BadUSB, bilgisayara klavye gibi davranır ve otomatik olarak tuş gönderir.  
İnsan yerine komut dosyası çalıştırdığı için saniyede çok daha fazla karakter yazabilir.

Super WiFi Duck ile cihazı WiFi üzerinden yönetebilir, scriptleri web arayüzünde oluşturup çalıştırabilirsiniz.  
Ek uygulama kurma, SD karta kopyalama veya her değişiklikte yeniden derleme gereksinimi azalır.

## Kullanım

1. Cihazı bilgisayara takın.
2. `wifiduck` adlı WiFi ağına bağlanın (varsayılan şifre: `wifiduck`).
3. Tarayıcıdan `192.168.4.1` adresini açın.
4. İlk Ducky Script dosyanızı yazın, kaydedin ve çalıştırın.
5. Güvenlik için `Settings` bölümünden SSID ve şifreyi değiştirin.

**Şifreyi unuttuysanız:**  
`Erase Flash: Sketch + WiFi Settings` seçeneğiyle yeniden yükleme yapın.

Ek sorular için proje issue sayfasını kullanabilirsiniz:  
https://github.com/spacehuhn/WiFiDuck/issues

## ESP32 S2/S3'e Yükleme

1. PlatformIO kurun.
2. Gerekirse `platformio.ini` dosyasını düzenleyin.
3. Kartınıza göre yükleyin:

```bash
# ESP32-S3
pio run -e esp32-s3-devkitc-1 -t upload

# ESP32-S2
pio run -e esp32-s2-kaluga-1 -t upload
```

4. Ardından [Kullanım](#kullanım) adımlarını uygulayın.

## Script Yazımı

### Temel Mantık

Komutlar tek boşlukla ayrılır.  
Aynı satırdaki tuşlar birlikte basılıp bırakılır.  
Metin yazdırmak için `STRING` komutu kullanılır.

| Örnek | Açıklama |
| --- | --- |
| `WINDOWS` + `r` (ayrı satırlar) | Önce Windows tuşu, sonra `r` |
| `WINDOWS r` | İki tuş aynı anda |
| `STRING WINDOWS r` | `WINDOWS r` metnini yazar |

### Fonksiyonlar

| Komut | Örnek | Açıklama |
| --- | --- | --- |
| `REM` | `REM Hello World!` | Yorum satırı |
| `DEFAULTDELAY` / `DEFAULT_DELAY` | `DEFAULTDELAY 200` | Komutlar arası bekleme (ms) |
| `DELAY` | `DELAY 1000` | Bekleme (ms) |
| `STRING` | `STRING Hello World!` | Sonraki metni yazar |
| `REPEAT` / `REPLAY` | `REPEAT 3` | Son komutu n kez tekrarlar |
| `LOCALE` | `LOCALE TR` | Klavye düzeni seçer |
| `KEYCODE` | `KEYCODE 0x02 0x04` | Belirli HID kodları gönderir |
| `LED` | `LED 40 20 10` | RGB LED rengini değiştirir (0-255) |

### Standart Tuşlar

| Tuşlar |
| --- |
| `a` - `z` |
| `A` - `Z` |
| `0` - `9` |
| `F1` - `F12` |

### Değiştirici Tuşlar

| Tuşlar |
| --- |
| `CTRL` / `CONTROL` |
| `SHIFT` |
| `ALT` |
| `WINDOWS` / `GUI` |

### Diğer Tuşlar

| Tuşlar |
| --- |
| `ENTER` |
| `MENU` / `APP` |
| `DELETE` |
| `HOME` |
| `INSERT` |
| `PAGEUP` |
| `PAGEDOWN` |
| `UP` / `UPARROW` |
| `DOWN` / `DOWNARROW` |
| `LEFT` / `LEFTARROW` |
| `RIGHT` / `RIGHTARROW` |
| `TAB` |
| `END` |
| `ESC` / `ESCAPE` |
| `SPACE` |
| `PAUSE` / `BREAK` |
| `CAPSLOCK` |
| `NUMLOCK` |
| `PRINTSCREEN` |
| `SCROLLLOCK` |

### Numpad Tuşları

| Tuşlar |
| --- |
| `NUM_0` - `NUM_9` |
| `NUM_ASTERIX` |
| `NUM_ENTER` |
| `NUM_MINUS` |
| `NUM_DOT` |
| `NUM_PLUS` |

### Örnek

```text
REM Hello World for Windows PCs
DEFAULTDELAY 200
GUI r
STRING notepad
ENTER
STRING Hello World!
```

## CLI Komutları

CLI arayüzü seri bağlantıdan (115200 baud, newline) veya web arayüzünden (`/terminal.html`) kullanılabilir.

### Genel

| Komut | Açıklama | Örnek |
| --- | --- | --- |
| `help` | Tüm komutları listeler | `help` |
| `ram` | Boş RAM miktarını verir | `ram` |
| `health` | Uptime, heap, SPIFFS ve script durumunu verir | `health` |
| `version` | Sürüm bilgisini verir | `version` |
| `settings` | Ayarları listeler | `settings` |
| `set <name> <value>` | Ayar değeri değiştirir | `set ssid "duck-net"` |
| `reset` | Ayarları varsayılana döndürür | `reset` |
| `status` | Script çalışma durumunu verir | `status` |
| `run <file>` | Script başlatır | `run /example.txt` |
| `stop <file>` | Script durdurur | `stop /example.txt` |

### SPIFFS Dosya Yönetimi

| Komut | Açıklama | Örnek |
| --- | --- | --- |
| `mem` | SPIFFS boyut/kullanım bilgisini verir | `mem` |
| `format` | SPIFFS formatlar | `format` |
| `ls <path>` | Dosyaları listeler | `ls /` |
| `create <file>` | Dosya oluşturur | `create /test.duck` |
| `remove <file>` | Dosya siler | `remove /test.duck` |
| `cat <file>` | Dosya içeriği gösterir | `cat /test.duck` |
| `rename <a> <b>` | Dosya adını değiştirir | `rename /a.duck /b.duck` |
| `write <file> <content>` | Dosyaya ekleme yapar | `write /a.txt "Hello"` |
| `stream <file>` | Akış modunda yazma açar | `stream /a.txt` |
| `close` | Akışı kapatır | `close` |
| `read` | Akıştan veri okur | `read` |

## Hata Ayıklama

Varsayılan debug modülü sınırlı olabilir.  
Gerekli durumlarda seri port üzerinden log almak için `ESP_LOGE`/`Serial` çıktısı kullanın.

## Geliştirme

### Web Dosyalarını Düzenleme

Web arayüz dosyaları `web/` klasöründedir (`.html`, `.css`, `.js`).

Değişiklikleri gömmek için:

```bash
python webconverter.py
```

Bu işlem `web/` içeriğini sıkıştırır ve `src/webfiles.h` dosyasını yeniden üretir.  
Sonrasında firmware'i tekrar yükleyin.

### Klavye Tanımlayıcısını Değiştirme

USB üretici/ürün kimliklerini `platformio.ini` içindeki derleme tanımlarından değiştirebilirsiniz.

Örnek:

```bash
-D USB_VID=0x05ac
-D USB_CUSTOM_PID=0x0267
-D USB_MANUFACTURER='"Apple Inc."'
-D USB_PRODUCT='"Apple Magic Keyboard"'
```

Ek USB VID/PID listeleri:  
https://devicehunt.com/view/type/usb/vendor/046D

### Klavye Düzeni (Locale) Çevirisi

Desteklenen düzenler `src/locale/` altındadır (US, DE, GB, ES, FR, RU, TR vb.).

Yeni düzen eklemek için özet adımlar:

1. `src/locale/` içinde mevcut bir locale dosyasını kopyalayın (ör. `locale_us.h`).
2. Dosya ve değişken adlarını yeni dil koduna göre değiştirin.
3. ASCII ve UTF-8 eşlemelerini kendi klavyenize göre düzenleyin.
4. `src/locales.h` içine yeni locale include satırını ekleyin.
5. `src/duckparser.cpp` içinde `LOCALE` komutuna yeni dil dalını ekleyin.
6. Tüm karakterleri içeren test scripti ile doğrulayın.

## Sorumluluk Reddi

Bu araç yalnızca eğitim, test ve farkındalık amaçlıdır.  
Yetkisiz kullanım, zarar verme veya kötüye kullanım amacıyla kullanılmamalıdır.

## Lisans

Bu proje MIT lisansı ile lisanslanmıştır.  
Detaylar için [LICENSE](LICENSE) dosyasına bakın.

## Teşekkürler

Bu projede kullanılan başlıca kütüphaneler:

- [Arduino](https://www.arduino.cc)
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
- [Adafruit DotStar](https://github.com/adafruit/Adafruit_DotStar)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [SimpleCLI](https://github.com/spacehuhn/SimpleCLI)
