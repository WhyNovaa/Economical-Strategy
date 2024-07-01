#include "bank.h"

void Bank:: setCreditDefaulters(const std::vector<Player>& defaulterList) {
    for(const auto &it: ){

    }
}

std::vector<Player*> Bank:: getCreditDefaulters() {

}

void Bank:: setInsuredPlayers(const std::vector<Player>& insuredList) {

}

std::vector<Player*>Bank:: getInsuredPlayers() {

}
//геттеры и сеттеры

Bank:: Bank(std::vector<Player>& p) : players(p) {}

void Bank::auction(std::vector<Player>& players, std::vector<std::pair<int, int>> offers) {

}//нужно прописать момент с балансом цен через просмотр баланса цен либо через понижение цены на условно 20%

bool Bank:: credit(Player& player) {

}//выдача кредита

std::vector<Player*> Bank:: checkCredits() {

} //проверка списка должников

bool Bank:: buyInsurance(Player& player) {

} //покупка страховки должника

std::vector<Player*> Bank:: getInsurance() {

} // получение списка застраховавшихся в этом месяце

void Bank:: resetInsurance(){

} // сброс списка застраховавшихся
