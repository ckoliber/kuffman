#ifndef KUFFMANPROCESS_H
#define KUFFMANPROCESS_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QThread>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtDebug>
#include <QDataStream>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

class KuffmanProcess : public QObject
{
    Q_OBJECT
public:
    explicit KuffmanProcess(QObject *parent = nullptr);
    void load();
    void counter();
    void header();
    void compress(QString path, QString target);
    void decompress(QString path, QString target);

    class Node{
        int data;
        int frequency;
        int min_frequency;
        Node *left = NULL;
        Node *right = NULL;
    public:
        Node(){}
        Node(int data, int frequency){
            qDebug() << data << endl;
            this->data = data;
            this->frequency = frequency;
            this->left = NULL;
            this->right = NULL;
        }
        Node(Node *node){
            this->data = node->data;
            this->frequency = node->frequency;
            this->min_frequency = node->min_frequency;
            this->left = node->left;
            this->right = node->right;
        }
        Node(Node *left, Node *right){
            this->frequency = left->frequency + right->frequency;
            this->left = left;
            this->right = right;
            this->min_frequency = (this->left->min_frequency < this->right->min_frequency) ? this->left->min_frequency : this->right->min_frequency;
        }

        void book(string *codebook, string code) {
            if(!left && !right){
                codebook[data] = code;
                return;
            }
            if(left){
                code += '0';
                left->book(codebook, code);
                code.erase(code.end()-1);
            }
            if(right){
                code += '1';
                right->book(codebook, code);
                code.erase(code.end()-1);
            }
        }
        bool operator> (Node &node){
            if(this->frequency > node.frequency){
                return true;
            }else if(this->frequency < node.frequency){
                return false;
            }else{
                return this->min_frequency > node.min_frequency;
            }
        }
    };
    class Heap{
        Node **minHeap;
        int heapSize;
    public:
        Heap(){
            heapSize = 0;
            minHeap = new Node*[257];
        }
        void push(Node *newNode) {
            int currentHeapNode = ++heapSize;
            while (currentHeapNode != 1 && *minHeap[currentHeapNode / 2] > *newNode) {
                minHeap[currentHeapNode] = minHeap[currentHeapNode / 2];
                currentHeapNode = currentHeapNode / 2;
            }
            minHeap[currentHeapNode] = newNode;
        }
        int size(){
            return heapSize;
        }
        void pop(){
            Node *lastNode = minHeap[heapSize];
            minHeap [heapSize--] = minHeap[1];
            int currentHeapNode = 1;
            int child = 2;

            while (child <= heapSize) {
                if (child < heapSize && *minHeap[child] > *minHeap[child + 1])
                    child++;

                if (*minHeap[child] > *lastNode)
                    break;

                minHeap[currentHeapNode] = minHeap[child];
                currentHeapNode = child;
                child *= 2;
            } // while not at end of heap

            minHeap[currentHeapNode] = lastNode;
        }
        Node * top(){
            return minHeap[1];
        }
    };

signals:
    void openProgress();
    void progressProgress(int progress);
    void closeProgress();

public slots:
    void compressStart();
    void decompressStart();

private :
    QString path;
    QString target;

    unsigned int frequencies[256] = {0};
    Node *huffman_tree;
    string huffman_book[256];
    string huffman_code = "";
};

#endif // KUFFMANPROCESS_H
