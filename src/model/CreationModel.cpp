#include "model/CreationModel.hpp"

void CreationModel::increment() {
    ++count;
    emit incremented(count);
}
