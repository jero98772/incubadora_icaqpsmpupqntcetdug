cube ([60,2,10]); 
//parte atras
translate([0,30,0])
cube ([60 ,2,47]); //60*43
//partre de arriba
cube ([60,30,2]);//(60*30)*2
translate([0,0,45])
    //la parte arriba
    cube ([60,30,2]);
    //pared izquierda
    cube ([2,30,45]);//(45*30)*2
 translate([58,0,0])
 //parte de del lado derecha
    cube ([2,30,45]);
    //parte ede la mitad
    translate([0,0,25])
    cube ([20,30,2]);
    //malla para retenar el calor un grado
        color("gray")
        translate([20,0,25])
    cube ([38,30,2]);
 translate([10,0,27])    
    cube ([2,30,18]);//(20*30)*1
    //parte aclilica
    color("black",0.7)
    translate([8,0,10])
    cube ([50,2,35]); 
        translate([0,-40,0])
 cube([10,2,45]);