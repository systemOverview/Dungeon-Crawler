#include "Constants.h"
bool Coordinates::operator==(const Coordinates& rhs) const {
    return (this->row == rhs.row && this->column == rhs.column);
}

bool Coordinates::operator!=(const Coordinates& rhs) const { return ((*this) == rhs) == false; }

bool Coordinates::operator<(const Coordinates& rhs) const {
    if (this->row == rhs.row) {
        return this->column < rhs.column;
    }
    else {
        return this->row < rhs.row;
    }
}

Coordinates::operator QString() const {
    return QString("Row: %1, Column : %2").arg(row).arg(column);
}

Coordinates Coordinates::CoordinatesSumClamped(const Coordinates& lhs,
                                               const Coordinates& rhs,
                                               int maxRow,
                                               int maxColumn,
                                               OverflowFlags& overflowFlags) {
    Coordinates sum = lhs + rhs;
    // TODO REFACTOR
    if (sum.row > maxRow) {
        sum.row = (sum.row % maxRow);
        overflowFlags.rowOverFlow = true;
    }

    if (sum.column > maxColumn) {
        sum.column = (sum.column % maxColumn);
        overflowFlags.columnOverFlow = true;
    }

    return sum;
}

Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs) {
    return {lhs.row + rhs.row, lhs.column + rhs.column};
}
Coordinates operator+(const Coordinates& lhs, const Coordinates::OverflowFlags& rhs) {
    return {lhs.row + rhs.rowOverFlow, lhs.column + rhs.columnOverFlow};
};

const inline static std::map<int, QString> PortalIdToPathRegister = {
        {1, ":/pics/textures/portal/portal1.png"},
        {2, ":/pics/textures/portal/portal2.png"},
        {3, ":/pics/textures/portal/portal3.png"},
};
