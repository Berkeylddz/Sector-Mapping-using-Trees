#include "SpaceSectorLLRBT.h"
#include <cmath>

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}


void removeR2(std::string &satir) {
    size_t pozisyon = satir.find('\r');
    if (pozisyon != std::string::npos) {
        satir.erase(pozisyon, 1);
    }
}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.

    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the file " << filename << std::endl;
        return;
    }
    std::string header;
    std::getline(inputFile, header); // İlk satırı oku ve atla

    int x, y, z;

    string line;
    while (getline(inputFile,line)){
        removeR2(line);

        std::istringstream iss(line);
        std::string x_str, y_str, z_str;

        // Satırdan x, y, z değerlerini al
        if (getline(iss, x_str, ',') && getline(iss, y_str, ',') && getline(iss, z_str, ',')) {
            // std::stoi fonksiyonu ile stringleri integer'a çevir
            x = std::stoi(x_str);
            y = std::stoi(y_str);
            z = std::stoi(z_str);

            insertSectorByCoordinates(x, y, z);
        }
    }

    inputFile.close();
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.


    double distance_from_earth = calculateDistanceFromEarth(x, y, z);

    int distance_component = static_cast<int>(distance_from_earth);

    // Coordinate components
    std::string x_component = (x == 0) ? "S" : (x > 0) ? "R" : "L";
    std::string y_component = (y == 0) ? "S" : (y > 0) ? "U" : "D";
    std::string z_component = (z == 0) ? "S" : (z > 0) ? "F" : "B";

    // Sektör kodunu oluştur
    std::string sector_code = std::to_string(distance_component) + x_component + y_component + z_component;

    Sector* newSector = new Sector(x, y, z);
    newSector->distance_from_earth = distance_from_earth;
    newSector->sector_code = sector_code;

    root = insert(root, newSector, nullptr);

    if(root->color){
        root->color = false;
    }

}

Sector* SpaceSectorLLRBT::insert(Sector* current, Sector* newSector, Sector* parent) {
    if (current == nullptr) {
        // Ağaç boşsa veya sona gelindi ise yeni düğümü ekleyerek return et
        newSector->color = true;  // Yeni eklenen düğümün rengini RED yap
        newSector->parent = parent; // Parent'ı belirle
        return newSector;
    }
    if(root->color){
        root->color = false;
    }


    // BST insert
    if (newSector->x < current->x || (newSector->x == current->x && (newSector->y < current->y || (newSector->y == current->y && newSector->z < current->z)))) {
        current->left = insert(current->left, newSector,current);
    } else {
        current->right = insert(current->right, newSector,current);
    }


    // LLRB özelliklerini kontrol et



    if (current->left != nullptr && current->left->color && current->right != nullptr && current->right->color) {
        flipColors(current);

    }

    if (current->right != nullptr && current->right->color) {
        current = rotateLeft(current);

    }
    if (current->left != nullptr && current->left->color && current->left->left != nullptr && current->left->left->color) {
        current = rotateRight(current);
    }

    if (current->left != nullptr && current->left->color && current->right != nullptr && current->right->color) {
        flipColors(current);

    }


    return current;
}


Sector* SpaceSectorLLRBT::rotateLeft(Sector* h) {
    Sector* x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = h->color;
    h->color = true;
    x->parent = h->parent; // x'in parent'ını güncelle
    h->parent = x; // h'nin parent'ını güncelle



    return x;
}

Sector* SpaceSectorLLRBT::rotateRight(Sector* h) {
    Sector* x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = h->color;
    h->color = true;
    x->parent = h->parent; // x'in parent'ını güncelle
    h->parent = x; // h'nin parent'ını güncelle

    return x;
}

void SpaceSectorLLRBT::flipColors(Sector* h) {
    h->color = true;
    h->left->color = false;
    h->right->color = false;

    // h'nin ve çocuklarının parent'larını güncelle
    h->left->parent = h;
    h->right->parent = h;

    /*
    // Eğer h'nin parent'ı varsa, h'nin parent'ının rengini değiştir
    if (h->parent != nullptr) {
        if (h->parent->color) {
            h->parent->color = false;
        } else {

            h->parent->color = true;
        }
    }*/
}



double SpaceSectorLLRBT::calculateDistanceFromEarth(int x, int y, int z) {
    return std::sqrt(x*x + y*y + z*z);
}



void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.

    std::cout << "\nSpace sectors inorder traversal:" << std::endl;
    displayInOrder(root);

}

void SpaceSectorLLRBT::displayInOrder(Sector* current) {
    if (current != nullptr) {
        displayInOrder(current->left);
        printSector(current);
        displayInOrder(current->right);
    }
}

// Yardımcı fonksiyon: Bir sektörü yazdırma
void SpaceSectorLLRBT::printSector(Sector* sector) {
    std::cout << (sector->color ? "RED " : "BLACK ") << "sector: " << sector->sector_code << std::endl;
}





void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.

    std::cout << "\nSpace sectors preorder traversal:" << std::endl;
    displayPreOrder(root);

}

void SpaceSectorLLRBT::displayPreOrder(Sector* current) {
    if (current != nullptr) {
        printSector(current);

        displayPreOrder(current->left);
        displayPreOrder(current->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.

    std::cout << "\nSpace sectors postorder traversal:" << std::endl;
    displayPostOrder(root);
}


void SpaceSectorLLRBT::displayPostOrder(Sector* current) {
    if (current != nullptr) {
        displayPostOrder(current->left);
        displayPostOrder(current->right);
        printSector(current);
    }
}









std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    // Earth sektörü ile başla
    Sector* start_sector = findSector("0SSS");
    // Hedef sektörü bul
    Sector* target_sector = findSector(sector_code);

    // Ulaşılabilirlik kontrolü
    if (start_sector == nullptr || target_sector == nullptr) {
        return path;
    }

    // Sektör yollarını bul

    findPathFromRoot(start_sector, target_sector, path);

    findPathFromParent(start_sector, target_sector, path);

    return path;
}

Sector* SpaceSectorLLRBT::findSector(const std::string& sector_code) {
    return findSectorRecursive(root, sector_code);
}

Sector* SpaceSectorLLRBT::findSectorRecursive(Sector* current, const std::string& sector_code) {
    // Eğer düğüm nullptr ise, sektörü bulamadık demektir.
    if (current == nullptr) {
        return nullptr;
    }

    // Sektör kodlarını karşılaştır
    if (sector_code == current->sector_code) {
        return current;
    }

    // Sol alt ağaca git
    Sector* left_result = findSectorRecursive(current->left, sector_code);
    if (left_result != nullptr) {
        return left_result;
    }

    // Sağ alt ağaca git
    return findSectorRecursive(current->right, sector_code);
}






// Yardımcı fonksiyon: Sektör yolu bulma
void SpaceSectorLLRBT::findPathFromRoot(Sector* start, Sector* target, std::vector<Sector*>& path) {
    if (start != nullptr && root->sector_code == "0SSS") {
        // Bu düğümü yolu path vektörüne ekle
        path.push_back(start);

        if (start->sector_code == target->sector_code) {
            // Hedef sektörü bulduk, işlemi sonlandır
            return;
        }

        // Hedef sektör, başlangıç sektöründen küçük ise, sola git
        findPathFromRoot(start->left, target, path);
        // Hedef sektör, başlangıç sektöründen büyük ise, sağa git
        findPathFromRoot(start->right, target, path);

        // Bu noktaya gelindiğinde, path vektörüne eklenen son sektör hedefe giden bir yol
        // değilse, yolu temizle (yol hedefe gitmiyorsa, bu kısmı sıfırlar)
        if (path.back()->sector_code != target->sector_code) {
            path.pop_back();
        }
    }
}


void SpaceSectorLLRBT::findPathFromParent(Sector* start, Sector* target, std::vector<Sector*>& path) {
    if (start != nullptr) {
        // Bu düğümü yolu path vektörüne ekle
        path.push_back(start);

        if (start->sector_code == target->sector_code) {
            // Hedef sektörü bulduk, işlemi sonlandır
            return;
        }

        // Parent üzerinden yukarı doğru ara
        Sector* parent = start->parent;
        while (parent != nullptr) {
            path.push_back(parent);

            // Hedefi bulduysak işlemi sonlandır
            if (target->sector_code == parent->sector_code) {
                return;
            }

            if(parent->right->sector_code == target->sector_code){
                path.push_back(parent->right);
                return;
            }

            if(parent->left->sector_code == target->sector_code){
                path.push_back(parent->left);
                return;
            }

            if(parent->right->right != nullptr && parent->right->right->sector_code == target->sector_code){
                path.push_back(parent->right->right);
                return;
            }

            if(parent->right->left != nullptr && parent->right->left->sector_code == target->sector_code){
                path.push_back(parent->right->left);
                return;
            }

            if(parent->left->left != nullptr && parent->left->left->sector_code == target->sector_code){
                path.push_back(parent->left->left);
                return;
            }

            if(parent->left->right != nullptr && parent->left->right->sector_code == target->sector_code){
                path.push_back(parent->left->right);
                return;
            }





            // Parent'ın parent'ına çık
            start = parent;
            parent = parent->parent;
        }

        // Bu noktaya gelindiğinde, path vektörüne eklenen son sektör hedefe giden bir yol
        // değilse, yolu temizle (yol hedefe gitmiyorsa, bu kısmı sıfırlar)
        if (path.back()->sector_code != target->sector_code) {
            path.pop_back();
        }
    }
}










void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    if (!path.empty()) {
        std::cout << "\nThe stellar path to Dr. Elara: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i]->sector_code;
            if (i < path.size() - 1) {
                std::cout << "->";
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "\nA path to Dr. Elara could not be found." << std::endl;

    }

}