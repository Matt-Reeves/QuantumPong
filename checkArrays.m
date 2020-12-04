clear;
clc
Mx = 800;
My = 600;
loadpath = '~/Desktop/';
j= 0 ;

psiR = dlmread([loadpath num2str(j) 'real.txt']);
psiI = dlmread([loadpath num2str(j) 'imag.txt']);
psiR = reshape(psiR,Mx,My);
psiI = reshape(psiI,Mx,My);
wavefunction = ((psiR + 1i*psiI).');
figure(999)
subplot(321)
imagesc(abs(ifft2(wavefunction)).^2)
colorbar

psiR = dlmread([loadpath num2str(j+1) 'real.txt']);
psiI = dlmread([loadpath num2str(j+1) 'imag.txt']);
psiR = reshape(psiR,Mx,My);
psiI = reshape(psiI,Mx,My);
wavefunction = ((psiR + 1i*psiI).');
figure(999)
subplot(322)
imagesc(abs((wavefunction)).^2)
colorbar

K2 = dlmread([loadpath num2str(j) 'K.txt']);
K2 = reshape(K2,Mx,My).';
figure(999)
subplot(323)
imagesc(fftshift(K2));
colorbar

V = dlmread([loadpath num2str(j) 'V.txt']);
V = reshape(V,Mx,My).';
figure(999)
subplot(324)
imagesc(V);
colorbar

b = dlmread([loadpath num2str(j+1) 'pixels.txt']);
b = reshape(b,Mx,My).';
subplot(325)
imagesc(b)
colorbar


