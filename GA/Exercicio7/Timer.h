#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

class Timer
{
public:
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       																								  //
//  Esta fun��o n�o cronometra tempo real, e sim incrementos que podem ser chamados	a qualquer momento.   //
//  Para um timer que roda junto com o loop, deve-se colocar a fun��o tick() ao final do loop.			  //
//																										  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Timer que cronometra conforme uso da fun��o tick(),
	Timer(int initial_time) { initialTime = initial_time; remainingTime = initial_time; }
	Timer() {}
	~Timer() {}

	//Seta o tempo inicial - N�o � necess�rio sempre, pois j� foi colocado no construtor
	inline void setInitialTime(int time) { initialTime = time; }

	//Seta o tempo restante - N�o � necess�rio sempre, pois isto � calculado dentro de outras fun��es
	inline void setRemainingTime(int time) { remainingTime = time; }

	//Retorna InitialTime
	inline int getInitialTime() { return initialTime; }

	//Retorna RemainingTime
	inline int getRemainingTime() { return remainingTime; }

	//Fun��o principal, deve ser chamada sempre que quiser passar 1 unidade de tempo
	inline void tick() { if (!over()) { remainingTime--; } }

	//Fun��o principal, deve ser chamada sempre que quiser passar X unidades de tempo
	inline void tick(int time) { if (!over()) { remainingTime -= time; } }

	//Retorna se o timer j� terminou
	inline bool over() { if (remainingTime <= 0) { return true; } else { return false; } }

	//Recome�a o timer com o parametro informado na primeira vez
	inline void restart() { remainingTime = initialTime; }

	//Recome�a o timer com novo valor de tempo
	inline void restart(int time) { remainingTime = time; initialTime = time; }


protected:
	int initialTime, remainingTime;

};

