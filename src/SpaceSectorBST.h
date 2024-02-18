#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    Sector* insert(Sector* current, Sector* newSector);
    double calculateDistanceFromEarth(int x, int y, int z);

    Sector* deleteSectorRecursive(Sector* current, int x, int y , int z);
    Sector* findMin(Sector* node);

    void displayInOrder(Sector* current);
    void printSector(Sector* sector);

    void displayPreOrder(Sector* current);
    void displayPostOrder(Sector* current);


    Sector* findSector(const std::string& sector_code);
    Sector* findSectorRecursive(Sector* current, const std::string& sector_code);

    void findPath(Sector* start, Sector* target, std::vector<Sector*>& path);

};

#endif // SPACESECTORBST_H
