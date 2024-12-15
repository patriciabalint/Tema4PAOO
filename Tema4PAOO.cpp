#include <iostream>
#include <memory> // Pentru std::shared_ptr și std::unique_ptr
#include <string>

// Item 13: Folosește obiecte pentru a gestiona resursele.
class ResourceManager {
    std::shared_ptr<std::string> resource; // Resursă gestionată prin std::shared_ptr

public:
    // Constructor
    ResourceManager(const std::string& value)
        : resource(std::make_shared<std::string>(value)) {
        std::cout << "Resursa '" << *resource << "' a fost creată și gestionată de ResourceManager.\n";
    }

    // Obținerea valorii resursei
    const std::string& getResource() const {
        return *resource;
    }

    // Destructor
    ~ResourceManager() {
        std::cout << "ResourceManager a distrus resursa '" << *resource << "'.\n";
    }
};

// Item 14: Gândește-te cu atenție la comportamentul de copiere pentru clasele care gestionează resurse.
class CopyControlled {
    std::unique_ptr<std::string> resource; // Resursa unică gestionată prin std::unique_ptr

public:
    // Constructor
    CopyControlled(const std::string& value)
        : resource(std::make_unique<std::string>(value)) {
        std::cout << "Resursa unică '" << *resource << "' a fost creată.\n";
    }

    // Constructor de mutare
    CopyControlled(CopyControlled&& other) noexcept
        : resource(std::move(other.resource)) {
        std::cout << "Resursa unică a fost mutată.\n";
    }

    // Operator de mutare
    CopyControlled& operator=(CopyControlled&& other) noexcept {
        if (this != &other) {
            resource = std::move(other.resource);
            std::cout << "Resursa unică a fost mutată prin operator=.\n";
        }
        return *this;
    }

    // Interzicem copierea
    CopyControlled(const CopyControlled&) = delete;
    CopyControlled& operator=(const CopyControlled&) = delete;

    // Afișarea valorii resursei
    void display() const {
        if (resource) {
            std::cout << "Resursa unică: " << *resource << "\n";
        } else {
            std::cout << "Resursa unică a fost mutată și este goală.\n";
        }
    }

    // Destructor
    ~CopyControlled() {
        if (resource) {
            std::cout << "Destructor: Resursa unică '" << *resource << "' a fost distrusă.\n";
        } else {
            std::cout << "Destructor: Nu mai există resursă de distrus.\n";
        }
    }
};

int main() {
    std::cout << "=== Demonstratie Item 13 ===\n";
    {
        ResourceManager manager("Resursa Principala");
        std::cout << "Resursa gestionată: " << manager.getResource() << "\n";
    } // manager iese din scope și resursa este eliberată automat.

    std::cout << "\n=== Demonstratie Item 14 ===\n";
    {
        CopyControlled cc1("Resursa Unica");
        cc1.display();

        // Mutăm resursa către alt obiect
        CopyControlled cc2 = std::move(cc1);
        cc2.display();
        cc1.display(); // Resursa originală este goală după mutare
    } // Destructorii sunt apelați automat.

    return 0;
}
