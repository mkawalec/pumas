plot '../build/hare_hensities_'.i.'.dat' matrix with image title sprintf("t=%.2f",i/10.)
i=i+1
if (i < n) reread
