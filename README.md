# Projekat 12 - Ostrva
##2D
Nacrtati pejzaž mora sa par ostrva. Nivo vode se mijenja tokom vremena. Na najvećem ostrvu
se nalazi palma i vatra. Plamen mijenja veličinu i boju tokom vremena. Aplikacija se pokreće
preko čitavog ekrana, a zatvara tasterom ESCAPE. Sunce se vidi na plavom nebu, i kreće se
po kružnoj putanji sa jednog kraja scene ka drugom, sve dok ne zađe. U istom trenutku, na
suprotnom kraju neba izlazi mjesec (realizovani bez tekstura) koji se kreće istom putanjom.
Nebo adekvatno mijenja boju.Oblaci se pojavljuju na jednom kraju neba i polako se kreću ka
suprotnom kraju tokom vremena, sve dok ne izađu sa ekrana, tokom čega se pojavljuju novi
oblaci. Ispred i iza ostrva se ajkule (vidljiva su im samo leđna peraja iznad površine vode) kreću
lijevo-desno tokom vremena, prateći nivo vode. Tasteri plus i minus ubrzavaju i usporavaju
vrijeme što ubrzava/usporava sve u sceni. Taster R vraća vrijeme na početnu vrijednost. Taster
1 prikazuje cijelu scenu preko tjemena, 2 preko ivica a 3 preko površina (default).
U proizvoljnom uglu ekrana napisati ime, prezime i indeks

##3D
Proširiti scenu na 3D sledećim stavkama (Prethodne funkcionalnosti projekta su i dalje
zadržane osim ukoliko ih naredne stavke ne mijenjaju):
- Broj indeksa, Ime i prezime studenta učitati kao providnu teksturu i smjestiti u ćošak
ekrana
- Svi objekti na sceni su sada 3D
- Na početku je aktivna izometrijska kamera:
- Kamera je postavljena u gornjem, prednjem, desnom uglu i okrenuta ka centru
scene
- Na početku je uključena aktivna ortogonalna projekcija sa proizvoljnim
parametrima takvim da je cijela scena vidljiva
- Uključeno je testiranje dubine
- Uključeno je odstranjivanje zadnjih lica
- Moguće je promijeniti projekciju na perspektivnu i obratno
- Vraćanje na ortogonalnu projekciju vraća kameru u prvobitnu poziciju i orjentaciju
- Kameru je moguće rotirati i pomjerati po X i Y osama
- Implementirati Blin-Fongov model osvjetljenja
- Ajkule sada kruže okolo ostrva
- Ajkule su učitane kao modeli
- Vatra na ostrvu je ujedno i tačkasto osvjetljenje
- Osvjetljenje vatre pulsira sa njenom promjenom veličine
- Ostrva imaju difuznu i spekularnu mapu pjeska
- Direkciono svjetlo koje predstavlja sunce i mjesec obasjava scenu
- Direkciono svjetlo mijenja boju, intenzitet i pravac u odnosu na doba dana
- Ispod listova palme je ljubičasto reflektorsko osvjetljenje koje je usmjereno ka najbližoj
ajkuli
- Reflektosko osvjetljenje prati kretanje ajkule
- More ima talase
- Oblaci su poluprovidni
