#pragma once
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Shader {
public:
	Shader() {
		loadShaders("BasicShader", "Basic.shader");
		loadShaders("RotationShader", "Rotation.shader");
		loadShaders("TextureShader", "Texture.shader");;
		setUniform1f("RotationShader", "angle", 0);
		setUniform1f("RotationShader", "offset", 0);
		setUniform1f("RotationShader", "radius", 0);
		
		loadShaders("3DShader", "3D.shader");

	}


	void loadShaders(const std::string& program, const std::string & filepath) {
		std::ifstream file(filepath);
		if (!file.is_open()) {
			std::cerr << "COULDN'T FIND SHADER FILE >>> INVALID FILE PATH" << std::endl;
			std::cerr << filepath << std::endl;
			return;
		}


		enum class MODE{VERTEX = 0, FRAGMENT = 1};
		std::stringstream s[2];
		std::string line;
		MODE mode;

		while (std::getline(file, line)) {
			if (line.find("VERTEX") != std::string::npos) {
				mode = MODE::VERTEX;
			}

			else if (line.find("FRAGMENT") != std::string::npos) {
				mode = MODE::FRAGMENT;
			}
			else {
				s[(int)mode] << line << "\n";
			}
			
		}
		

		createProgram(program, s[0].str(), s[1].str());
	}
	bool validProgram(const std::string & program) {
		if (programs.find(program) != programs.end()) {
			return true;
		}
		return false;
	}
	void bind(const std::string program)  {
		if (validProgram(program)) {
			glUseProgram(programs[program]);
		}
		else {
			std::cout << "NOT VALID PROGRAM <<< " << std::endl;
		}
		
	}

	void unbind() const {
		glUseProgram(0);
	}

	void setUniform4f(const std::string program, const std::string & uName, float f1, float f2, float f3, float f4) {
		bind(program);
		glUniform4f(findUniformLocation(program, uName), f1, f2, f3, f4);

	}
	void setUniformMatrix4fv(const std::string program, const std::string& uName, float * matrix) {
		bind(program);
		glUniformMatrix4fv(findUniformLocation(program, uName),  1, GL_FALSE, matrix);
	}

	void setUniform1f(const std::string program, const std::string& uName, float value) {
		bind(program);
		glUniform1f(findUniformLocation(program, uName), value);

	}
	void setUniformi(const std::string program, const std::string & uName, unsigned int texUnit) {
		bind(program);
		glUniform1i(findUniformLocation(program, uName), texUnit);

	}
	int & findUniformLocation(const std::string program, const std::string & uName) {
		if (locations.find(uName) != locations.end()) {
			return locations[uName];
		}

		int location = glGetUniformLocation(programs[program], uName.c_str());
		if (location == -1) {
			std::cerr << "COULD NOT LOCATE UNIFROM <<< UNIFORM NAME: " << uName << std::endl;

		}

		locations[uName] = location;
		return location;

	}
	~Shader() {
		for (auto &it : programs) {
			glDeleteProgram(it.second);
		}
		
	}
private:

	unsigned int compileShader(unsigned int type, const std::string & shader) {
		unsigned int shaderId = glCreateShader(type);

		const char* source = shader.c_str();
		glShaderSource(shaderId, 1, &source, nullptr);
		glCompileShader(shaderId);


		int result;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
		if (result != GL_TRUE) {
			int length;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);

			char* message = new char[length];
			glGetShaderInfoLog(shaderId, length, nullptr, message);
			std::cerr << "SHADER COMPILATION ERROR >>>>  FOR " << ((type == GL_VERTEX_SHADER) ? "VERTEX " : "FRAGMENT ") << "SHADER" << std::endl;
			std::cerr << message << std::endl;
			
			
			delete[] message;
			glDeleteShader(shaderId);
			return 0;
		}

		return shaderId;
	}
	void createProgram(const std::string & program,const std::string& vertexShader, const std::string& fragmentShader) {
		programs[program] = glCreateProgram();

		unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(programs[program], vs);
		glAttachShader(programs[program], fs);
		glLinkProgram(programs[program]);
		glValidateProgram(programs[program]);

		glDeleteShader(vs);
		glDeleteShader(fs);

	}

	
	
private:
	std::unordered_map<std::string, int> locations;
	std::unordered_map<std::string, unsigned int> programs;
	
};