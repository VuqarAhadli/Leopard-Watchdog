# Pardus System Security & Health Check

**Modüler bir Pardus GNU/Linux sistemi güvenlik ve sağlık kontrol aracı.** Sistem riski otomatik olarak tarar, yapılandırma sorunlarını tanımlar ve kullanıcıya net bir tanı raporu sunar.

## 🎯 Amaç

`pardus-health-check`, Pardus sistemleri için **kapsamlı bir güvenlik ve sistem sağlığı değerlendirme** aracıdır. Dört ana modülden oluşan işlem hattı:

1. **APT Paket Analizi** - Eski, desteklenmeyen, güvenlik açığı olan paketleri tespit
2. **Açık Port Taraması** - Ağ güvenliğini değerlendir, riskli portları işaretle
3. **Donanım Envanteri** - Bilinmeyen cihazları, sürücüleri ve olağandışı bileşenleri tespit
4. **Sistem Sağlığı** - CPU, bellek, disk, servisler, loglar kapsamlı kontrol

**Çıkış:** Renkli CLI, JSON, veya HTML rapor formatlarında güvenlik skoru (0-100) ve uygulanabilir öneriler.

## 📋 Özellikler

✅ **Modüler Tasarım** - Her kontrol bağımsız modül olarak çalışır  
✅ **Renkli CLI Çıkış** - Terminal'de okunması kolay raporlar  
✅ **Machine-Readable JSON** - Otomasyona uygun veri formatı  
✅ **Web-Viewable HTML** - Tarayıcıda görüntülenebilir raporlar  
✅ **Risk Skoru Hesaplama** - 0-100 skadasında güvenlik puanı  
✅ **Uygulanabilir Öneriler** - Her bulguya karşı spesifik çözüm önerileri  
✅ **Hafif ve Hızlı** - C yazılı, minimal bağımlılık  
✅ **Pardus Optimized** - Debian tabanlı sisteme özel kontroller  

## 🔧 Kurulum

### Ön Koşullar

```bash
sudo apt update
sudo apt install build-essential cmake git
```

### Kaynaktan Derleme

```bash
git clone https://github.com/yourusername/pardus-health-check.git
cd pardus-health-check
mkdir build
cd build
cmake ..
make
sudo make install
```

### Debian Paketi Oluşturma

```bash
cd build
cpack -G DEB
sudo dpkg -i pardus-health-check-1.0.0-Linux.deb
```

## 🚀 Kullanım

### Temel Tarama (Tüm Moduller)

```bash
pardus-health-check --all
```

Çıkış:
```
┓     ┏┓┏┓┏┓┏┓┳┓┳┓    
┃ ┣ ┃┃┃┃┣┫┣┫┃┃    
┗┛┗┛┗┛┣┛┛┗┛┛┻┛    
┓ ┏┏┓┏┳┓┏┓┓┏┳┓┏┓┏┓
┃┃┃┣┫ ┃ ┃ ┣┫┃┃┃┃┃┓
┗┻┛┛┗ ┻ ┗┛┛┗┻┛┗┛┗┛

╔═══════════════════════════════════════════════════════════╗
║  Pardus System Security & Health Check                   ║
╚═══════════════════════════════════════════════════════════╝

OVERALL SECURITY SCORE: 82.5/100
✓ System is in GOOD condition

Score Breakdown:
  Package Health:  85.0/100
  Network Safety:  78.0/100
  Hardware:        88.0/100
  System Health:   82.0/100
```

### JSON Raporunu Kaydet

```bash
pardus-health-check --all --json health-report.json
```

### HTML Raporunu Oluştur

```bash
pardus-health-check --all --html health-report.html
# Tarayıcıda aç: firefox health-report.html
```

### Spesifik Kontroller

```bash
pardus-health-check --packages      # Sadece paket kontrolü
pardus-health-check --network       # Sadece port taraması
pardus-health-check --hardware      # Sadece donanım kontrolü
pardus-health-check --system        # Sadece sistem sağlığı
```

### Verbose Mod

```bash
pardus-health-check --all --verbose
```

### Yardım

```bash
pardus-health-check --help
```

## 📊 Rapor Formatları

### CLI Rapor (Varsayılan)
Renkli, terminal-friendly çıkış:
- Güvenlik skoru
- Bulguların özeti
- Spesifik öneriler
- Zaman damgası

### JSON Rapor
Machine-readable format, otomasyona uygun:
```json
{
  "timestamp": 1710123456,
  "overall_score": 82.5,
  "scores": {
    "package": 85.0,
    "network": 78.0,
    "hardware": 88.0,
    "system": 82.0
  },
  "summary": {
    "total_findings": 5,
    "critical": 0,
    "high": 1,
    "medium": 2,
    "low": 2
  },
  "findings": [...]
}
```

### HTML Rapor
Tarayıcı-dostu formatında:
- Stilize edilmiş skor göstergesi
- Renk kodlanmış bulgu kartları
- Responsive tasarım
- Web'de paylaşmaya hazır

## 📈 Risk Seviyeleri

| Seviye | Renk | Anlamı |
|--------|------|--------|
| 🚨 CRITICAL | Kırmızı | Acil işlem gereklidir |
| ⚠️ HIGH | Kırmızı | Önemli sorun |
| ⚡ MEDIUM | Sarı | İlgilenilmeli |
| ℹ️ LOW | Sarı | İzlenmeli |
| ℹ️ INFO | Mavi | Bilgilendirici |

## 🔍 Her Modül Ne Kontrol Eder?

### 1. APT Paket Analizi
- Güvenlik güncellemeleri yok
- Hala geçerli mi? Depo bilgileri ile karşılaştırma
- Yıkık paket bağımlılıkları
- Eski/yerinden çıkarılmış paketler (python2, apache2, vb.)

### 2. Açık Port Taraması
- ss/netstat ile aktif portları enumera
- Bilinen portlar (SSH, HTTP, HTTPS, Database, vb.) risk seviyesine göre kategorilendir
- Dünya çapında açık veritabanı portları uyarısı
- Listelenenden daha fazla port = saldırı yüzeyini azalt

### 3. Donanım Envanteri
- USB cihazları numaralandır
- Çekirdek modüllerini kontrol et
- Bilinmeyen PCI cihazlarını tespit et
- BIOS/UEFI güncellemeleri öner
- Çıkarılabilir depolamayı kontrol et

### 4. Sistem Sağlığı
- CPU kullanımı (% >75 uyarı)
- Bellek (% >80 uyarı)
- Disk kullanımı (% >80 uyarı, >90 kritik)
- Başarısız systemd servisleri
- Log dosyalarında hata desenleri

## 🛠️ Kod Mimarisi

```
src/
├── main.c                  # Ana program, CLI yönetimi
├── logger.c               # Merkezi logging, renkli çıkış
├── ui.c                   # Banner, UI elemanları
├── system_health.c        # CPU/Mem/Disk/Services modülü
├── package_analyzer.c     # APT paket analizi
├── port_scanner.c         # Açık port taraması
├── hardware_inventory.c   # Donanım envanteri
└── report_generator.c     # JSON/HTML/CLI rapor üretimi

include/
└── pardus_health_check.h  # Başlık dosyası, yapılar, fonksiyon imzaları
```

## 📝 Lisans

MIT License - Pardus Topluluğuna açık olarak sunulmuştur

## 🤝 Katkı

1. Repository'yi fork et
2. Feature branch'i oluştur (`git checkout -b feature/AmazingFeature`)
3. Değişiklikleri commit et (`git commit -m 'Add AmazingFeature'`)
4. Branch'e push et (`git push origin feature/AmazingFeature`)
5. Pull Request aç

## 📧 İletişim

Sorular veya öneriler için: [email] veya [community forum]

---

**Pardus ile Güvenli ve Sağlıklı Sistem Yönetimi!** 🔒
