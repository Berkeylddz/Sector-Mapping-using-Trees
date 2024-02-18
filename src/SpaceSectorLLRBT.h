#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);


    double calculateDistanceFromEarth(int x, int y, int z);

    Sector* insert(Sector* current, Sector* newSector,Sector* parent);

    Sector* rotateLeft(Sector* h);
    Sector* rotateRight(Sector* h);
    void flipColors(Sector* h);

    void displayInOrder(Sector* current);
    void printSector(Sector* sector);

    void displayPreOrder(Sector* current);
    void displayPostOrder(Sector* current);

    Sector* findSector(const std::string& sector_code);
    Sector* findSectorRecursive(Sector* current, const std::string& sector_code);
    void findPath(Sector* start, Sector* target, std::vector<Sector*>& path);

    void findPathFromRoot(Sector* start, Sector* target, std::vector<Sector*>& path);
    void findPathFromParent(Sector* start, Sector* target, std::vector<Sector*>& path);
};

#endif // SPACESECTORLLRBT_H
