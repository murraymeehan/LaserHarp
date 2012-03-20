clear
close all


%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Cylinder paramters: 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%N=50;C1=[0 0 1];C2=[3 3 3];CR=1; 
% should return false: Inputs: 
%P=[312.134888,252.007706,821.570312], C1=[171.225281,296.035126,1117.386719], C2=[189.734222,269.359375,1447.929199], CR=[100.000000]
% should return true: Inputs: 
P=[176.973129,264.271057,1245.454590], C1=[171.225281,296.035126,1117.386719], C2=[189.734222,269.359375,1447.929199], CR=[100.000000]

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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
