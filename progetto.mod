set T; #coppie (source ,destination)
set K; #tutti i percorsi disponibili
set Kt{T} within K; #percorsi possibili tra una coppia (s,d)	(indexed sets)
set M; #tipi di modulazioni possibili   																
set K1K2 within K cross K; #solo le coppie di cammini che condividono almeno un percorso

param Q; #costante positiva maggiore del numero massimo di transceivers --> domanda massima dell'istanza / banda minima (50)
param G; #dimensione guardband --> 12,5ghz o  1
param B; #lunghezza di banda di un transceiver --> 37,5gh o 3
param S; #tutto lo spettro disponibile (4000ghz oppure 4000/12,5)
param Lam{M,K} binary;	#1 se la portata della modulazione soddisfa la lunghezza del percorso
param r{M}; #capacità di un transceiver che opera a una modulazione m (0, 50gb/s; 1, 100; ...)
param d{T}; #domanda di traffico da una coppia (s,d)

var dk1k2{K1K2} binary; #1 se k2 viene prima di k1
var Smax; #spettro massimo utilizzato nella soluzione specifica
var f{K}; #frequenze di inizio
var alpha1 integer; #fattori di moltiplicazione
var alpha2 integer;
var b{M,K} integer; #numero di coppie di transceiver (uno su s e uno su d) con modulazione m che vengono usate su un percorso k
var Beta{M,K} binary; #1 se la modulazione m viene usata sul traffico t sul percorso k

#minimizzare somma pesata tra spettro usato e transceiver usati
minimize z: alpha1 * Smax + alpha2 * (1/card(T)) * B * sum{m in M} sum{k in K} b[m,k];

s.t. primo_vincolo: sum{m in M} sum{k in K} Beta[m,k] = 1;

s.t. secondo_vincolo{m in M, k in K}: Lam[m,k] * Q * Beta[m,k] >= b[m,k];

s.t. terzo_vincolo{t in T}: sum{m in M} sum{k in Kt} r[m] * b[m,k] >= d[t];

s.t. percorso_minimo{k in K}: f[k] + sum{m in M} B * b[m, k] + G <= Smax;

s.t. due_percorsi{(k1,k2) in K1K2}: f[k1] + sum{m in M} B * b[m, k1] + G - f[k2] <= S * dk1k2[(k1,k2)];
 
s.t. tre_percorsi{(k1,k2) in K1K2}: f[k2] + sum{m in M} B * b[m, k2] + G - f[k1] <= S * (1- dk1k2[(k1,k2)]);
