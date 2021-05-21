set K; #tutti i percorsi disponibili
set M; #tipi di modulazioni possibili   
set K1K2 within K cross K; #solo le coppie di cammini che condividono almeno un percorso	

param B; #lunghezza di banda di un transceiver --> 37,5gh o 3
param S; #tutto lo spettro disponibile (4000ghz oppure 4000/12,5)
param b_star{M,K};
param beta_star{M,K};
param G; #dimensione guardband --> 12,5ghz o  1
param Smax_star;
param beta = 1;

var w{K1K2} >= 0;
var dk1k2{K1K2} binary; #0 se f(k1) < f(k2) € K1K2
var f{K} >= 0;
var w_min;

maximize z: w_min;

s.t. vincolo_w_min{(k1,k2) in K1K2}: w_min <= w[k1,k2];

s.t. settimo_vincolo{k in K}: f[k] + sum{m in M} B * b_star[m,k] + G <= beta * Smax_star;

s.t. ottavo_vincolo{(k1,k2) in K1K2}: f[k1] + sum{m in M} B * b_star[m,k1] + G - f[k2] <= S * dk1k2[k1,k2];

s.t. nono_vincolo{(k1,k2) in K1K2}: f[k2] + sum{m in M} B * b_star[m,k2] + G - f[k1] <= S * (1-dk1k2[k1,k2]);

s.t. decimo_vincolo{(k1,k2) in K1K2}: w[k1,k2] <= f[k1] - f[k2] - sum{m in M} B * b_star[m,k2] - G + S * (1-dk1k2[k1,k2]);

s.t. undicesimo_vincolo{(k1,k2) in K1K2}: w[k1,k2] <= f[k2] - f[k1] - sum{m in M} B * b_star[m,k1] - G + S * dk1k2[k1,k2];