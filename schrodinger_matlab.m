Lx = 400;
Ly = 300;
Mx = 800;
My = 600;
dx = Lx/Mx;
dy = Ly/My;
x = -Lx/2:dx:Lx/2-dx;
y = -Ly/2:dy:Ly/2-dy;
[X,Y] = meshgrid(x,y);

dkx = 2*pi/Lx;
dky = 2*pi/Ly;
kxmax = 2*pi/dx;
kymax = 2*pi/dy;
kx = -kxmax/2:dkx:kxmax/2-dkx;
ky = -kymax/2:dky:kymax/2-dky;
[Kx,Ky] = meshgrid(kx,ky);

K2 = single(fftshift(0.5*(Kx.^2 + Ky.^2)));


Nt = 1e2;
dt = 1.5e-1;

sigma = 8;
x0 = -50;
k0 = 4;
V0 = 20;
psi0 = exp(-(X-x0).^2/sigma^2 - (Y-x0).^2/sigma^2).*exp(1i*k0*(X+Y));
psi = single(psi0);

V =  V0*(abs(X) < 4 & (abs(Y) > 60 | abs(Y) < 8));
V = fftshift(fft2(V));
V = V.*exp(-(Kx.^2+Ky.^2)/0.25/sigma^2 );
V = single(real(ifft2(fftshift(V))));
figure(1)
imagesc(V)
tic
psi = fft2(psi);
for ii = 1:Nt
    psi = exp(-1i*K2*dt).*psi;
    psi = fft2(exp(-1i*V*dt).*ifft2(psi));
    psi = exp(-1i*K2*dt).*psi;
    if floor(ii/2) == ii/2
        figure(998)
        imagesc(x,y,abs(ifft2(psi)).^2)
        set(gca,'plotBoxAspectRatio',[400/300 1 1],'ydir','normal')
        pause(0.01)
    end

end
toc
