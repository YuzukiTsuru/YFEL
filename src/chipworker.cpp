#include "chipworker.h"
#include "chips/chip_db.h"

ChipWorker::ChipWorker(chip_version_t chip_version) : chip_version(chip_version) {
    generate_chip_db();
}

void ChipWorker::generate_chip_db() {
    chip_db.push_back(new d1(chip_version));
    chip_db.push_back(new r528(chip_version));
}

void ChipWorker::check_chip() {
    for (auto item: chip_db) {
        item->chip_detect();
    }
}
