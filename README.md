# SRS Case Study - Sensör Veri İşleme Programı
## Programı Derleme ve Çalıştırma Talimatları
1. C++14 uyumlu bir derleyiciye (örneğin, GCC, __MSVC__) sahip olduğunuzdan emin olun.
2. Program dosyalarının bulunduğu dizine terminal üzerinden gidin.
3. Programı aşağıdaki komut ile derleyin:
   ```sh
   cl /EHsc Srs.cpp
4. Programı çalıştırmak için:
   ```sh
   Srs.exe
### Geliştirme Sırasında Yapılan Varsayımlar
* Sensör, belirtilen minimum ve maksimum aralıkta tam sayı değerleri üretir.
* Veri üretim aralığı sabit olarak 500 milisaniyedir.
* Sonuçlar, çalışma dizininde bulunan results.txt adlı bir dosyaya kaydedilir.
### Tasarım Tercihleri
1. Nesne Yönelimli Yaklaşım (OOP):
   * Program, kapsülleme, kalıtım ve çok biçimlilik gibi OOP prensiplerine güçlü bir vurgu yapılarak tasarlanmıştır.
   * __Sensor__ temel sınıfı, gelecekte yeni sensör tiplerinin kolayca eklenmesine olanak sağlar.
2. Sorumluluk Ayrımı:
   * __TempSensor__ sınıfı veri üretimiyle ilgilenirken, __DataProcessor__ sınıfı verilerin işlenmesi ve kaydedilmesini sağlar.
3. Dosya Kaydı:
   * Sonuçlar, kalıcı depolama ve izlenebilirlik için *results.txt* dosyasına kaydedilir.
4. Kod Modülerliği:
   * Veri üretimi, işleme ve kaydetme gibi işlemler birbirinden ayrılarak kodun bakımı ve genişletilmesi kolaylaştırılmıştır.
### Ekstra Özellikler
1. Dinamik Dosya İşleme:
   * Program, *results.txt* dosyasının varlığını kontrol eder. Dosya yoksa oluşturur ve sağlam bir kayıt sistemi sağlar.
2. Kullanıcı Dostu Giriş/Çıkış:
   * Kaç adet veri üretileceği kullanıcı tarafından belirlenir.
   * Program çalışırken gerçek zamanlı olarak veri toplama işlemleri konsolda gösterilir.
