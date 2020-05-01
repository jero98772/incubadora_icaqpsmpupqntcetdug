
difference(){
    //--estructura principal
union() {
    //--techo
 translate ([-60,-30,70])
    cube ([122,60,2 ]);
    //--repisa 3 piso
    translate ([-60,-30,30])
    cube ([122,60, 2]);
     //--REPISA DE LA MITAD 2 PISO
   translate ([-60,-30,0])
    cube ([122,60, 2]);
    //--repiisa inferior
translate ([-60,-30,-70])
      cube ([122,60,2 ]);
    //-- pared trasera
  translate ([-60,30,-70])
    color("orange")
    cube ([122,4,142]);
    //-- pared lado  izquierdo
      translate ([-62,-30,-70])
    color("orange")
    cube ([2,60,144]);
    //-- 2 pared  lado  izquierdo  mas  derecho de bajo
    translate ([-43,-30,2])
    color("orange")
    cube ([2,60,68])
        //-- 2 pared  lado  izquierdo  mas  derecho de arriba
    translate ([-62,-30,30])
    color("orange")
    cube ([2,60,30]);
}
//--desparision de una esquina
translate([60,-30,-72])
rotate([0,0,75])
    cylinder (r =20/2  , h =144,$fn=3);
}
//--pilar de  lado derecho
module modulopilar() {
 color("orange")
translate([60,-23,-70])
rotate([0,0,225])
   cube ([10,2,142]);
translate([44.5,-32,-70])
     color("orange")
cube ([10,2,142]);
translate([60,-23,-70])
rotate([0,0,90])
     color("orange")
   cube ([10,2,142]); 
    }
    modulopilar();
            //-- repisa 2
    translate ([-80,-30,15])
    cube ([38,60,2]);
    //-- repisa  4
    translate ([-80,-30,45])
    cube ([38,60,2]);
    module reja() {
        for (i=[0:veces]){
        color("gray"){
        translate ([cordenada_x,cordenada_y,cordenada_z])
            translate([i*iy,i*ix,0])
      rotate ([rotaciongradox,rotaciongradoy,0]){
        cube([1,1,altura]);

          }}}}
    //--reja 
  reja(altura= 140,cordenada_x=60,cordenada_y=-15,cordenada_z=-68,veces=14,rotaciongradoy=0,rotaciongradox=0,iy=0,ix=3);
   //--reja galoineritomini
  reja(altura= 70,cordenada_x=-58,cordenada_y=-30,cordenada_z=-68,veces=37,rotaciongradoy=-0,rotaciongradox=0,iy=3,ix=0);
        //--acrilico incubadora y nacedora
          translate([-60,-31,2]){
              color("black",0.7)
           cube ([112,0.3,70]);
          }
          //--retoques
          //--poner el aserrin  abjo desplegar
          //-- frontal
 translate([-60,-30,-70]){
           cube ([110,2,5]);
          }
         //-- lado 
          translate([59,-17,-70]){
           cube ([2,50,5]);
          }
          //--puerta
         distanciadelapuerta =90;//87 para habrir y 63 para cerrar
                        zrotar=30;
            translate([distanciadelapuerta,-25,-65]){
          mirror(){
          rotate([0,0,-zrotar]){//rotar 30 para habrir 0 para cerrar
           cube ([2,60,50]);
          }}}
          