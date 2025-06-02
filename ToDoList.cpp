/*  Anggota kelompok:
    1. Jennifer Jestina - 245150301111022
    2. Khaliqa Adin Laily Andrika Putri - 245150301111023
    3. Ika Luthfiamuda Aryanti - 245150301111026
    4. Rizqi Surya Ardiansyah - 245150300111033
    5. Zahra Azalea Siregar - 245150307111035
    6. Siti Nur Azzahra Sabilla - 245150307111037
*/

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// struct untuk menyimpan data tugas
struct Task { 
    string id;
    string nama;
    int prioritas;
    string deadline;
    string deskripsi;
    string kategori;
    Task* next;
};

// linked list untuk menyimpan tugas-tugas
Task* front = nullptr;
int idCounter = 1;

// membuat id tugas otomatis (tugas1, tugas2, dst)
string generateId() {
    return "TUGAS" + to_string(idCounter++); // format id tugas
}

// membandingkan dua deadline, true jika d1 lebih awal dari d2
bool isEarlier(string d1, string d2) {
    return d1 < d2;
}

// validasi format deadline agar sesuai yyyy-mm-dd dan hanya angka
bool validDeadline(const string& deadline) {
    if (deadline.length() != 10) return false; // harus 10 karakter
    if (deadline[4] != '-' || deadline[7] != '-') return false; // harus ada '-' di posisi 4 dan 7
    // cek apakah semua karakter yang seharusnya angka adalah angka
    for (int i = 0; i < 4; ++i) if (deadline[i] < '0' || deadline[i] > '9') return false; // tahun
    for (int i = 5; i < 7; ++i) if (deadline[i] < '0' || deadline[i] > '9') return false; // bulan
    for (int i = 8; i < 10; ++i) if (deadline[i] < '0' || deadline[i] > '9') return false; // tanggal
    // cek apakah tahun, bulan, dan tanggal valid
    int tahun = stoi(deadline.substr(0,4)); // ambil tahun dari string
    int bulan = stoi(deadline.substr(5,2)); // ambil bulan dari string
    int tanggal = stoi(deadline.substr(8,2)); // ambil tanggal dari string
    if (tahun < 0 || bulan < 1 || bulan > 12 || tanggal < 1 || tanggal > 31) return false; // tahun tidak boleh negatif, bulan 1-12, tanggal 1-31
    return true; // jika semua valid, return true
}

/////////////////////////////////////////////////////////////

// menambah tugas baru ke priority queue sesuai prioritas dan deadline
void tambahTugas() {
    Task* baru = new Task(); // alokasi memori untuk tugas baru
    baru->id = generateId(); // generate id otomatis
    cout << "Nama Tugas: "; getline(cin >> ws, baru->nama);
    cout << "Prioritas (1=tinggi, 2=sedang, 3=rendah): "; cin >> baru->prioritas; // input prioritas
    string deadline;
    do {
        cout << "Deadline (YYYY-MM-DD): "; cin >> deadline; // input deadline
        if (!validDeadline(deadline)) { // validasi format deadline
            cout << "Format deadline salah! Masukkan ulang (YYYY-MM-DD, hanya angka, tidak negatif).\n";
        }
    } while (!validDeadline(deadline)); // ulangi sampai format benar
    baru->deadline = deadline; // simpan deadline yang sudah divalidasi
    cout << "Deskripsi: "; getline(cin >> ws, baru->deskripsi); // input deskripsi tugas

    // memilih kategori dari daftar yang tersedia
    int pilihKategori; 
    cout << "Pilih Kategori:\n";
    cout << "  1. Akademik\n";
    cout << "  2. Organisasi\n";
    cout << "  3. Lain-lain\n";
    do {
        cout << "Masukkan nomor kategori: ";
        cin >> pilihKategori;
        if (cin.fail() || pilihKategori < 1 || pilihKategori > 3) { // validasi input kategori
            cout << "Pilihan tidak valid. Pilih 1, 2, atau 3.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    } while (cin.fail() || pilihKategori < 1 || pilihKategori > 3); // ulangi sampai input valid

    if (pilihKategori == 1) baru->kategori = "Akademik";
    else if (pilihKategori == 2) baru->kategori = "Organisasi";
    else baru->kategori = "Lain-lain";

    baru->next = nullptr; // inisialisasi next pointer

    // memasukkan tugas ke priority queue
    // jika queue kosong atau tugas baru lebih prioritas dari tugas paling depan
    if (!front || baru->prioritas < front->prioritas ||
        (baru->prioritas == front->prioritas && isEarlier(baru->deadline, front->deadline))) {
        baru->next = front;
        front = baru; // tugas baru menjadi depan queue
    } else {
        // mencari posisi yang tepat untuk tugas baru
        Task* curr = front;
        while (curr->next &&
            (curr->next->prioritas < baru->prioritas ||
            (curr->next->prioritas == baru->prioritas && !isEarlier(baru->deadline, curr->next->deadline)))) {
            curr = curr->next;
        } // jika tugas baru harus disisipkan di tengah atau akhir
        baru->next = curr->next; // tugas baru akan disisipkan setelah curr 
        curr->next = baru;
    }
    cout << "Tugas berhasil ditambahkan! ID: " << baru->id << endl;
}

//////////////////////////////////////////////////////////////////

// menampilkan seluruh tugas yang ada di queue
void tampilkanTugas() {
    if (!front) { // jika queue kosong
        cout << "Tidak ada tugas.\n";
        return;
    }
    cout << "Daftar Tugas:\n";
    Task* curr = front; // mulai dari tugas paling depan
    int no = 1; // nomor urut tugas
    while (curr) {
        cout << no++ << ". ID: " << curr->id << "\n";
        cout << "   Nama: " << curr->nama << "\n";
        cout << "   Prioritas: " << curr->prioritas << "\n";
        cout << "   Deadline: " << curr->deadline << "\n";
        cout << "   Deskripsi: " << curr->deskripsi << "\n";
        cout << "   Kategori: " << curr->kategori << "\n";
        curr = curr->next;
    }
}

// mencari tugas berdasarkan id
Task* cariTugas(string id) {
    Task* curr = front; // mulai dari tugas paling depan
    while (curr) {
        if (curr->id == id) return curr; // jika id cocok, return tugas tersebut
        curr = curr->next; // lanjut ke tugas berikutnya
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////

void updateTugas() {
    cout << "Masukkan ID tugas yang ingin diupdate: ";
    string id; cin >> id;
    Task* t = cariTugas(id);
    if (!t) {
        cout << "Tugas tidak ditemukan.\n";
        return;
    }

    // simpan pointer ke node yang akan diupdate
    Task* curr = front;
    Task* prev = nullptr;
    while (curr && curr != t) {
        prev = curr;
        curr = curr->next;
    }

    // hapus node dari queue
    if (!prev) front = curr->next;
    else prev->next = curr->next;

    // update data tugas
    cout << "Update Nama Tugas (sebelumnya: " << t->nama << "): "; getline(cin >> ws, t->nama);
    cout << "Update Prioritas (sebelumnya: " << t->prioritas << "): "; cin >> t->prioritas;
    string deadline;
    do {
        cout << "Update Deadline (sebelumnya: " << t->deadline << "): ";
        cin >> deadline;
        if (!validDeadline(deadline)) {
            cout << "Format deadline salah! Masukkan ulang (YYYY-MM-DD, hanya angka, tidak negatif).\n";
        }
    } while (!validDeadline(deadline));
    t->deadline = deadline;
    cout << "Update Deskripsi (sebelumnya: " << t->deskripsi << "): "; getline(cin >> ws, t->deskripsi);

    int pilihKategori;
    cout << "Pilih Kategori Baru:\n";
    cout << "  1. Akademik\n";
    cout << "  2. Organisasi\n";
    cout << "  3. Lain-lain\n";
    do {
        cout << "Masukkan nomor kategori: ";
        cin >> pilihKategori;
        if (cin.fail() || pilihKategori < 1 || pilihKategori > 3) {
            cout << "Pilihan tidak valid. Pilih 1, 2, atau 3.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    } while (cin.fail() || pilihKategori < 1 || pilihKategori > 3);

    if (pilihKategori == 1) t->kategori = "Akademik";
    else if (pilihKategori == 2) t->kategori = "Organisasi";
    else t->kategori = "Lain-lain";
    t->next = nullptr;

    // masukkan kembali node ke queue sesuai prioritas dan deadline baru
    if (!front || t->prioritas < front->prioritas ||
        (t->prioritas == front->prioritas && isEarlier(t->deadline, front->deadline))) {
        t->next = front;
        front = t;
    } else {
        Task* temp = front;
        while (temp->next &&
            (temp->next->prioritas < t->prioritas ||
            (temp->next->prioritas == t->prioritas && !isEarlier(t->deadline, temp->next->deadline)))) {
            temp = temp->next;
        }
        t->next = temp->next;
        temp->next = t;
    }

    cout << "Tugas berhasil diupdate!\n";
}

// menghapus tugas berdasarkan id
void hapusTugas() {
    cout << "Masukkan ID tugas yang ingin dihapus: ";
    string id; cin >> id; // input id tugas
    Task* curr = front; // mulai dari tugas paling depan
    Task* prev = nullptr;
    // mencari tugas berdasarkan id
    while (curr && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }
    // jika tugas tidak ditemukan, tampilkan pesan error
    if (!curr) {
        cout << "Tugas tidak ditemukan.\n";
        return;
    }
    // jika tugas ditemukan, hapus dari linked list
    if (!prev) front = curr->next;
    else prev->next = curr->next;
    delete curr;
    cout << "Tugas berhasil dihapus!\n";
}

///////////////////////////////////////////////////////////////////

// menampilkan detail tugas berdasarkan id yang dicari
void cariTugasMenu() {
    cout << "Masukkan ID tugas yang dicari: ";
    // input id tugas yang ingin dicari
    string id; cin >> id;
    Task* t = cariTugas(id);
    // jika tugas tidak ditemukan, tampilkan pesan error
    if (!t) {
        cout << "Tugas tidak ditemukan.\n";
        return;
    }
    // jika tugas ditemukan, tampilkan detail tugas
    cout << "ID: " << t->id << "\n";
    cout << "Nama: " << t->nama << "\n";
    cout << "Prioritas: " << t->prioritas << "\n";
    cout << "Deadline: " << t->deadline << "\n";
    cout << "Deskripsi: " << t->deskripsi << "\n";
    cout << "Kategori: " << t->kategori << "\n";
}

// mengekspor data tugas ke file csv agar bisa dibuka di excel
void eksporCSV() {
    // membuat file csv dan menulis header
    ofstream fout("tugas.csv");
    fout << "ID,Nama,Prioritas,Deadline,Deskripsi,Kategori\n";
    Task* curr = front;
    // menulis setiap tugas ke file csv
    while (curr) {
        fout << curr->id << ",";
        fout << "\"" << curr->nama << "\",";
        fout << curr->prioritas << ",";
        fout << curr->deadline << ",";
        fout << "\"" << curr->deskripsi << "\",";
        fout << "\"" << curr->kategori << "\"\n";
        curr = curr->next;
    }
    fout.close();
    cout << "Data tugas berhasil diekspor ke tugas.csv!\n";
}

// mengekspor data tugas ke file txt dengan format tabel agar rapi di notepad
void eksporTXT() {
    ofstream fout("tugas.txt");
    // menulis header tabel
    fout << "+--------+----------------------+-----------+------------+----------------------+-------------+\n";
    fout << "|   ID   |        Nama          | Prioritas |  Deadline  |      Deskripsi       |  Kategori   |\n";
    fout << "+--------+----------------------+-----------+------------+----------------------+-------------+\n";
    Task* curr = front;
    // menulis setiap tugas ke file txt dengan format tabel
    while (curr) {
        fout << "| " << curr->id;
        for (int i = curr->id.length(); i < 6; ++i) fout << " ";
        fout << " | " << curr->nama;
        for (int i = curr->nama.length(); i < 20; ++i) fout << " ";
        fout << " |     " << curr->prioritas << "     ";
        fout << " | " << curr->deadline << " ";
        fout << "| " << curr->deskripsi;
        for (int i = curr->deskripsi.length(); i < 20; ++i) fout << " ";
        fout << " | " << curr->kategori;
        for (int i = curr->kategori.length(); i < 11; ++i) fout << " ";
        fout << " |\n";
        curr = curr->next;
    }
    fout << "+--------+----------------------+-----------+------------+----------------------+-------------+\n";
    fout.close();
    cout << "Data tugas berhasil diekspor ke tugas.txt!\n";
}

/////////////////////////////////////////////////////////////////////////////

// menampilkan menu utama dan menangani pilihan user
void menu() {
    int pilih;
    do {
        cout << "\n========================================\n";
        cout << "            TO DO LIST <3                \n";
        cout << "      Kerjain dulu, baru rebahan       \n";
        cout << "========================================\n";
        cout << "1. Tambah Tugas\n";
        cout << "2. Tampilkan Semua Tugas\n";
        cout << "3. Update Tugas\n";
        cout << "4. Hapus Tugas\n";
        cout << "5. Cari Tugas\n";
        cout << "6. Keluar\n";
        cout << "7. Ekspor ke Excel (CSV)\n";
        cout << "8. Ekspor ke TXT (Tabel)\n";
        cout << "Pilih menu: "; cin >> pilih; cin.ignore();
        // validasi input pilihan menu
        switch (pilih) {
            case 1: tambahTugas(); break;
            case 2: tampilkanTugas(); break;
            case 3: updateTugas(); break;
            case 4: hapusTugas(); break;
            case 5: cariTugasMenu(); break;
            case 6: cout << "Keluar...\n"; break;
            case 7: eksporCSV(); break;
            case 8: eksporTXT(); break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilih != 6);
}

// fungsi utama untuk menjalankan program
int main() {
    menu(); // panggil fungsi menu untuk menampilkan menu utama
    cout << "Tekan Enter untuk keluar..."; // pesan sebelum keluar
    cin.get(); // tunggu input Enter dari user
    return 0;
}