#include "kuffmanprocess.h"

KuffmanProcess::KuffmanProcess(QObject *parent) : QObject(parent){}

/**
1. Open Target File
2. Scan all files by a hashmap and count the bytes
3. Create Huffman Tree
4. Deload Kuffman at first of file
5. Start streamers
**/

void KuffmanProcess::load(){
    Heap heap;
    Node *node;
    for(int i = 0; i < 256; i++){
        if (frequencies[i]){
            node = new Node(i, frequencies[i]);
            heap.push(node);
        }
    }
    Node *node1;
    Node *node2;
    Node *merged;
    while(heap.size() > 1){
        node1 = heap.top();
        heap.pop();
        node2 = heap.top();
        heap.pop();
        merged = new Node(node1, node2);
        heap.push(merged);
    }
    huffman_tree = heap.top();
    huffman_tree->book(huffman_book,huffman_code);
}

void KuffmanProcess::counter(){
    bzero(frequencies,256*4);

    emit openProgress();
    QFile file(this->path);
    file.open(QIODevice::ReadOnly);
    QTextStream reader(&file);

    char file_data;
    long counter = 0;
    while(!reader.atEnd()){
        reader >> file_data;
        frequencies[file_data+128]++;
        counter++;
        if(counter%128 == 0){
            emit progressProgress((counter*100)/file.size());
        }
    }
    file.close();
    emit closeProgress();

    QFile target(this->target);
    target.open(QIODevice::WriteOnly);
    QTextStream writer(&target);
    for(int i = 0; i < 256; i++){
        writer << (char) (0x000000ff & frequencies[i]);
        writer << (char) ((0x0000ff00 & frequencies[i]) >> 8);
        writer << (char) ((0x00ff0000 & frequencies[i]) >> 16);
        writer << (char) ((0xff000000 & frequencies[i]) >> 24);
    }
    target.close();
}
void KuffmanProcess::header(){
    emit openProgress();
    QFile file(this->path);
    file.open(QIODevice::ReadOnly);
    QTextStream reader(&file);
    for(int i = 0; i < 256; i++){
        char c1,c2,c3,c4;
        reader >> c1 >> c2 >> c3 >> c4;
        frequencies[i] = 0;
        frequencies[i] |= ((c1) & 0x000000ff);
        frequencies[i] |= ((c2 << 8) & 0x0000ff00);
        frequencies[i] |= ((c3 << 16) & 0x00ff0000);
        frequencies[i] |= ((c4 << 24) & 0xff000000);
    }
    file.close();
    emit closeProgress();
}

void KuffmanProcess::compress(QString path, QString target){
    this->path = path;
    this->target = target;
    QThread *thread = new QThread;
    this->moveToThread(thread);
    connect(thread,&QThread::started,this,&KuffmanProcess::compressStart);
    thread->start();
}
void KuffmanProcess::decompress(QString path, QString target){
    this->path = path;
    this->target = target;
    QThread *thread = new QThread;
    this->moveToThread(thread);
    connect(thread,SIGNAL(started()),this,SLOT(decompressStart()));
    thread->start();
}

void KuffmanProcess::compressStart(){
    KuffmanProcess::counter();
    KuffmanProcess::load();

    for(int a = 0 ; a < 256 ; a++){
        qDebug() << QString::fromStdString(huffman_book[a]) << "\t" << a << endl;
    }

    emit openProgress();
    char next_data = 0;
    char next_byte = 0;
    int bit_counter = 0;

    long counter = 0;

    QFile file(this->path);
    file.open(QIODevice::ReadOnly);
    QTextStream reader(&file);

    QFile target(this->target);
    target.open(QIODevice::Append);
    QTextStream writer(&target);

    while(!reader.atEnd()){
        reader >> next_data;
        for(int i = 0; i < huffman_book[next_data+128].size(); i++){
            if(bit_counter > 7){
                writer << next_byte;
                next_byte = 0;
                bit_counter = 0;
            }
            bit_counter++;
            next_byte <<= 1;
            next_byte |= (huffman_book[next_data+128][i] == '1' ? 0x1 : 0x0);
        }
        counter++;
        if(counter % 128 == 0){
            emit progressProgress(counter*100/file.size());
        }
    }
    if(bit_counter){
        writer << next_byte;
    }

    file.close();
    target.close();

    emit closeProgress();
}
void KuffmanProcess::decompressStart(){
    KuffmanProcess::header();
    KuffmanProcess::load();



    for(int a = 0 ; a < 256 ; a++){
        qDebug() << QString::fromStdString(huffman_book[a]) << "\t" << a << endl;
    }

    emit openProgress();

    char next_byte = 0;
    long counter = 0;

    QFile file(this->path);
    file.open(QIODevice::ReadOnly);
    QTextStream reader(&file);
    reader.seek(256*4);

    QFile target(this->target);
    target.open(QIODevice::WriteOnly);
    QTextStream writer(&target);

    while(!reader.atEnd()){
        reader >> next_byte;
        for(int i = 7; i >= 0; i--){
            if((next_byte >> i) & 0x1){
                huffman_code += '1';
            }else{
                huffman_code += '0';
            }
            for(int j = 0; j < 256; j++){
                if(huffman_book[j] == huffman_code){
                    if(frequencies[j]){
                        int k = j-128;
                        char result = (k & 0x000000ff);
                        writer << result;
                        huffman_code.clear();
                        frequencies[j]--;
                        break;
                    }else{
                        return;
                    }
                }
            }
        }
        counter++;
        if(counter % 128 == 0){
            emit progressProgress(counter*100/file.size());
        }
    }

    file.close();
    target.close();

    emit closeProgress();
}
