/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include "raytracer.h"

#include <thread>         // std::thread
#include <mutex>          // std::mutex
std::mutex mtx0;           // mutex for critical section

using namespace std;



int main(int argc, char* argv[])
{
	//-----
	// Ask the user for the shading style to apply
	
	string entry = "";
	int maxChoice = 0;   // part A can apply up to 3 styles, part B have 7 
	int count = 0;
	while(true){
		std::cout << "Currently availiable styles:\n";
		std::cout << "(A1)   Ambient only\n";
		std::cout << "(A12)  Ambient + Diffuse\n";
		std::cout << "(A123) Phong Illumination (Ambient + Diffuse + Specular)\n";
		std::cout << "(B1)   Recursive ray tracing\n";
		std::cout << "(B2)   Hard shadows\n";
		std::cout << "(B3)   Add a cylinder to the scene\n";
		std::cout << "(B4)   Anti-Aliasing\n";
		std::cout << "(B5)   Soft-shadow\n";
		std::cout << "(B6)   Motion-Blur\n";
		std::cout << "(B7)   Texture mapping\n";
		std::cout << "(B8)   Depth of field\n";
		std::cout << "(C)    Bonus\n";
		std::cout << "Please enter a string listed above in the bracket to apply the rendering styles:\n";
		std::cin >> entry;
		
		maxChoice = 0;
		if(entry.find("A") != std::string::npos){
			maxChoice = 3;
		}else if(entry.find("B") != std::string::npos){
			maxChoice = 8;
		}
		
		for(int i = 1; i < maxChoice + 1; i++){
			if(entry.find(std::to_string(i)) != std::string::npos){
				count++;
			}
		}
		
		if(entry.find("C") != std::string::npos){
			count++;
		}

		if(count > 0){
			break;
		}
	}
	
	//-----
	
	
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.  
	Raytracer raytracer;
	LightList light_list;
	Scene scene;   

	int width = 320;
	int height = 240;

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}
	
// -------------- bonus --------------
	if(entry.find("C") != std::string::npos){
		entry.append("B12");
		Material blue(Color(0.0, 0.0, 0.6), Color(0.0,0.4,0.1),    // table
			Color(0.628281, 0.555802, 0.366065),
			12);

		Material red(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),    // bat cylinder
			Color(0.628281, 0.555802, 0.366065),
			51.2);


		SceneNode* table = new SceneNode(new UnitCube(), &blue);
		scene.push_back(table);


		double factor2[3] = { 6.0, 4.0, 1.0 };
		table->translate(Vector3D(0, 0, -4));
		table->rotate('z', 40);
		table->rotate('y', 17);
		table->rotate('x', -25);
		table->scale(Point3D(0, 0, 0), factor2);

		PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
		light_list.push_back(pLight);


		Camera camera1(Point3D(0, 0, 3), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
		Camera camera2(Point3D(3, 2, 1), Vector3D(-4, -2, -6), Vector3D(-0.2, 2.7, 1.5), 60.0);

		Image image1(width, height);
		raytracer.render(camera1, scene, light_list, image1, entry); //render 3D scene to image
		image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

		// Render it from a different point of view.
		Image image2(width, height);
		raytracer.render(camera2, scene, light_list, image2, entry);
		image2.flushPixelBuffer("view2.bmp");

	
		// Free memory
		for (size_t i = 0; i < scene.size(); ++i) {
			delete scene[i];
		}

		for (size_t i = 0; i < light_list.size(); ++i) {
			delete light_list[i];
		}
	
		return 0;
	}

//



	// Define materials for shading.
	Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),     // Ellipsoid
		Color(0.628281, 0.555802, 0.366065),
		51.2);
	Material gold2(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),    // Sphere
		Color(0.628281, 0.555802, 0.366065),
		51.2);
	Material gold3(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),    // Cylinder
		Color(0.628281, 0.555802, 0.366065),
		51.2);
	Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),                    // Plane
		Color(0.316228,0.316228,0.316228),
		12.8);
		
	Material gold4(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),    // Cube
		Color(0.628281, 0.555802, 0.366065),
		51.2);


	
	
	//-----
	if(entry.find(std::to_string(7)) != std::string::npos){
		jade.setTexture("rubics.bmp", 332, 355);
		//gold.setTexture("rubics.bmp", 332, 355);
		//gold2.setTexture("rubics.bmp", 332, 355);
		//jade.setTexture("earthmap.bmp", 1000, 500);
		gold.setTexture("earthmap.bmp", 1000, 500);
		gold2.setTexture("earthmap.bmp", 1000, 500);
		gold4.setTexture("star.bmp", 500, 500);
	}
	
	
	
	//-----
	int soft_shadow_enabled = 0;
	if(entry.find("B") != std::string::npos){
		if(entry.find("5") != std::string::npos){
			soft_shadow_enabled = 1;
		}	
	}
	
	if(!soft_shadow_enabled){
		// Defines a point light source.
		PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
		light_list.push_back(pLight);
	}else{		
		double gap = 0.3;
		int xRange = 5;
		int yRange = 5;
		// approximate area light by uniformly distributing light sources
		for(int n = -xRange; n <= xRange; n++){
			for(int m = -yRange; m <= yRange; m++){
				// Defines a point light source.
				PointLight* pLight = new PointLight(Point3D(gap * n, gap * m,5), Color(0.9,0.9,0.9));
				light_list.push_back(pLight);
			}
		}	
		
		
	}
	//-----
	
	// Add a unit square into the scene with material mat.
	SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
	if(entry.find("7") == std::string::npos){
		scene.push_back(sphere);
	}
	SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
	scene.push_back(plane);

	// Apply some transformations to the sphere and unit square.
	double factor1[3] = { 1.0, 2.0, 1.0 };
	sphere->translate(Vector3D(0, 0, -5));
	sphere->rotate('x', -45);
	sphere->rotate('z', 45);
	sphere->scale(Point3D(0, 0, 0), factor1);

	double factor2[3] = { 6.0, 6.0, 6.0 };
	plane->translate(Vector3D(0, 0, -7));
	plane->rotate('z', 45);
	plane->scale(Point3D(0, 0, 0), factor2);
	
	
	//----- 
	//Add unit cylinder into the scene if it is requested by the user 
	if(entry.find("B") != std::string::npos){
		if(entry.find("3") != std::string::npos){
			
			SceneNode* cylinder = new SceneNode(new UnitCylinder(), &gold3);
			scene.push_back(cylinder);
			
			double factor3[3] = { 1.2, 1.5, 1.6 };

			cylinder->translate(Vector3D(-3, 0, -5));	
			cylinder->scale(Point3D(0, 0, 0), factor3);
			//cylinder->rotate('x', 45);
			//cylinder->rotate('y', 90);
		}
	}
	
	//-----
	
	
	SceneNode* sphere2 = new SceneNode(new UnitSphere(), &gold2);
	scene.push_back(sphere2);

	double factor4[3] = { 1.0, 1.0, 1.0 };
	//sphere2->translate(Vector3D(0, 3, -5));
	sphere2->translate(Vector3D(2, 2.5, -5));
	//sphere2->translate(Vector3D(2, 2, -5));
	//sphere2->translate(Vector3D(1, 1, -5));
	sphere2->scale(Point3D(0, 0, 0), factor4);
	
	
	
	//----- 
	//Add unit cylinder into the scene if it is requested by the user 
	if(entry.find("B") != std::string::npos){
		if(entry.find("7") != std::string::npos){
			SceneNode* cube = new SceneNode(new UnitCube(), &gold4);
			scene.push_back(cube);
			double factor5[3] = { 2.0, 2.0, 2.0 };

			cube->translate(Vector3D(0, 0, -5));	
			cube->rotate('y', -30);
			cube->rotate('x', 20);
			cube->scale(Point3D(0, 0, 0), factor5);
	
		}
	}
	
	
	// Render the scene, feel free to make the image smaller for
	// testing purposes.
	Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
	Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
	
	if(entry.find("6") == std::string::npos){
		
		Image image1(width, height);
		raytracer.render(camera1, scene, light_list, image1, entry); //render 3D scene to image
		image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

		// Render it from a different point of view.
		Image image2(width, height);
		raytracer.render(camera2, scene, light_list, image2, entry);
		image2.flushPixelBuffer("view2.bmp");
	}
	

	
	double rBuf[height * width];
	double gBuf[height * width];
	double bBuf[height * width];
	
	double rBuf2[height * width];
	double gBuf2[height * width];
	double bBuf2[height * width];
	for(int i = 0; i < (height * width); i++){
		rBuf[i] = 0.0;
		gBuf[i] = 0.0;
		bBuf[i] = 0.0;
		
		rBuf2[i] = 0.0;
		gBuf2[i] = 0.0;
		bBuf2[i] = 0.0;
	}
	
	
	if(entry.find("6") != std::string::npos){  //motion_blur_enabled
		Image image3(width, height);
		Image image4(width, height);
		int motion_count = 20;
		
		double maxTranslate = 0.025;
		double minTranslate = -0.025;
		double xDisplacement = 0.0;
		double yDisplacement = 0.0;
		double zDisplacement = 0.0;
		
		int movement_count = 0;
		
		for(int t = 0; t < motion_count; t++){
			mtx0.lock();
			
			double randX = (rand() / RAND_MAX) * (maxTranslate - minTranslate) + minTranslate;
			double randY = (rand() / RAND_MAX) * (maxTranslate - minTranslate) + minTranslate;
			double randZ = 0.1 * ((rand() / RAND_MAX) * (maxTranslate - minTranslate) + minTranslate);
			
			xDisplacement += randX;
			yDisplacement += randY;
			zDisplacement += randZ;
			
			//sphere2->translate(Vector3D(0, 0.025, 0.0));
			if((entry.find("B") != std::string::npos) && (entry.find("7") != std::string::npos)){
				sphere2->translate(Vector3D(randX, randY, randZ));
			}else{
				sphere->translate(Vector3D(randX, randY, randZ));
			}
			
			movement_count++;
			
			// Render it from a different point of view.
			Image imageFrame(width, height);
			Image imageFrame2(width, height);
			raytracer.render(camera1, scene, light_list, imageFrame, entry);
			raytracer.render(camera2, scene, light_list, imageFrame2, entry);
			
			#pragma omp parallel for
			for(int i = 0; i < height; i++){
				#pragma omp parallel for
				for(int j = 0; j < width; j++){
					rBuf[i*width+j] += imageFrame.rbuffer[i*width+j];
					gBuf[i*width+j] += imageFrame.gbuffer[i*width+j];
					bBuf[i*width+j] += imageFrame.bbuffer[i*width+j];
					
					rBuf2[i*width+j] += imageFrame2.rbuffer[i*width+j];
					gBuf2[i*width+j] += imageFrame2.gbuffer[i*width+j];
					bBuf2[i*width+j] += imageFrame2.bbuffer[i*width+j];
				}
			}

			mtx0.unlock();
			
		}
		
		
		#pragma omp parallel for
		for(int i = 0; i < height; i++){
			#pragma omp parallel for
			for(int j = 0; j < width; j++){
				image3.rbuffer[i*width+j] = int(rBuf[i*width+j]/(movement_count * 1.0));
				image3.gbuffer[i*width+j] = int(gBuf[i*width+j]/(movement_count * 1.0));
				image3.bbuffer[i*width+j] = int(bBuf[i*width+j]/(movement_count * 1.0));
				
				image4.rbuffer[i*width+j] = int(rBuf2[i*width+j]/(movement_count * 1.0));
				image4.gbuffer[i*width+j] = int(gBuf2[i*width+j]/(movement_count * 1.0));
				image4.bbuffer[i*width+j] = int(bBuf2[i*width+j]/(movement_count * 1.0));
			}
		}
		if((entry.find("B") != std::string::npos) && (entry.find("7") != std::string::npos)){
			sphere2->translate(Vector3D(-xDisplacement, -yDisplacement, -zDisplacement)); 
		}else{
			sphere->translate(Vector3D(-xDisplacement, -yDisplacement, -zDisplacement)); 
		}
		image3.flushPixelBuffer("view1.bmp");
		image4.flushPixelBuffer("view2.bmp");
		
	}
	
	


	// Free memory
	for (size_t i = 0; i < scene.size(); ++i) {
		delete scene[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}
	
	return 0;
}