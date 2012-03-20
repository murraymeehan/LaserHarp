clear
close all


%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Cylinder paramters: 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
N=50;

% Sample points:
% should return false:
P=[412.595154,239.261536,1289.312866]; C1=[171.225281,296.035126,1117.386719]; C2=[189.734222,269.359375,1447.929199]; CR=[100.000000];

% should return true:
%P=[172.339005,291.587616,1241.938110]; C1=[171.225281,296.035126,1117.386719]; C2=[189.734222,269.359375,1447.929199]; CR=[100.000000];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Generate points around the ends of a cylinder
% 	Author: Ryan Derry
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Z=C2-C1;
Z2=Z-[0 0 1]; % breaks if C1 or C2 is [0 0 -1]
r1=CR*cross(Z,Z2)/norm(cross(Z,Z2));
r2=CR*cross(Z,r1)/norm(cross(Z,r1));
C=zeros(N,3); 

for n=1:N/2       
	C(n,:) = C1 + r1*sin(4*pi*n/N) + r2*cos(4*pi*n/N);      
end 
for n=N/2+1:N       
	C(n,:) = C2 + r1*sin(4*pi*n/N) + r2*cos(4*pi*n/N);       
end    

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Plot the cylinder
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure()
hold on
scatter3(C(:,1),C(:,2),C(:,3))
scatter3([C1(1) C2(1)], [C1(2) C2(2)], [C1(3) C2(3)], 'r')
plot3([P(1) 0], [P(2) 0], [P(3) 0],'r');
hold off

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Check if P is inside cylinder
% 	given handpos(P) and cylinder(C1,C2,CR)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CN1 = (C2 - C1) / norm(C2 - C1);
fDistanceToPlane1 = dot( P-C1, CN1 )
fDistanceToPlane2 = dot( P-C2, -CN1 )
if (fDistanceToPlane1 < 0 ) 
	result = false
elseif (fDistanceToPlane2 < 0) 
	result = "false @ elseif (dot( P-C2, -CN1 ) < 0) " 
else
	TempP =  P - (CN1 * fDistanceToPlane1);
	fDistanceFromCenter = norm( TempP-C1)
	if (fDistanceFromCenter > CR)
		result = false
	else
		result = true
	endif
endif

