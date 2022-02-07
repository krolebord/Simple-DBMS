#include "../algorithms.h"

int findFirstRowIndex(const IndexEntry* items, int itemsCount, int id) {
    int low = 0, high = itemsCount - 1;

    int result = -1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (id == items[mid].id) {
            result = mid;
            high = mid - 1;
        }
        else if (id < items[mid].id) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }

    return result;
}
