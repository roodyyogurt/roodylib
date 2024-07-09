!  This file to be included somewhere before verbstub.g

verb "smell", "sniff", "inhale", "breathe"
	*                                       DoSmell
	* object                                DoSmell

! Roody's note: I've found that if you support "swim", players will also try
! "swim in [object]"
verb "swim", "dive"
	*                                       DoSwim
	* object                                DoSwim
	* "in"/"into"/"to" object                          DoSwim

verb "wave"
	*                                       DoWaveHands
	* "hands"                               DoWaveHands
	* "to"/"at" object                      DoWaveHands
	* held                                  DoWave