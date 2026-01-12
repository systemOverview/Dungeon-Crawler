//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_TERMINALUI_H
#define PRAK_TERMINALUI_H
#include "AbstractUI.h"
class Level;
class TerminalUI : public AbstractUI, public AbstractController
{
public:
    void draw(Level *) override;
    std::pair<int, int> move() override;
    std::pair<int, int> translateMove(int step) override;
};
#endif //PRAK_TERMINALUI_H
