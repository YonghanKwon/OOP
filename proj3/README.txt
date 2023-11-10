//소프트웨어학부 20186889 권용한
//객체지향프로그래밍 02분반
//written by visual studio 2019
//OS type : windows 10
//compiler version c++14
//to compile to code, double click the VirtualLego.sln and connect to directX
//and run local debuger(press F5)
//to execute run the VirtualLego.exe or run VirtualLego.sln and press ctrl+F5
//Modification : I modify the ball number, and velocity to 3 fixed and ballUpdate's rate to 1
//so the ball's velocity don't descrease.
//and added checker to CSphere to check the ball is hit
//if the ball is hit by the other ball, the ball's member variable checker become 0
//and if the checker is 0 than the ball is not drawn, and hasIntersected function doesn't work
//and member function getCheck is added, and it is true when checker=1 and false checker=0
//the white ball is the last of sphere array so when the white ball is hit with red ball, the red ball is become
//this and white ball is &ball, so only this's checker become 0 and &ball's checker remain 1
//and if blue ball and white ball has intersected, blue ball's checker can become 0
//so after blue ball and white ball hit, modify blue ball and white ball's checker to 1
//and mouse LButton's function is erased, so the view point is fixed
//mouse RButton can only modify the blue ball's x coord and fixed z coord
//the ball hit wall, the ball's velocity of x or z become its negative value
//and if the ball'z coord is under -4.5+ball's Radius, the ball's velocity became 0 and the ball's x, z coord to
//ordinary(0,0)