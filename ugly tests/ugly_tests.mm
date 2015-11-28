//
//  ugly_tests.m
//  ugly tests
//
//  Created by Chris Bush on 2015-11-26.
//
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>


#include "ugly.h"

#include "glfw_app.h"


#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"



#define EXPECT_THROW(stmt, ...) {\
  bool threw = false; \
  try { stmt; } catch(...) { threw = true; } \
  XCTAssert(threw, __VA_ARGS__); }


@interface ugly_tests : XCTestCase

@end

glfwApp *theApp;

gl::Context *context;

@implementation ugly_tests

// Once, before entire test run.
+ (void)setUp {
}

// Once, after entire test run.
+ (void)tearDown {
}


// Before each test.
- (void)setUp {
  [super setUp];
  theApp = new glfwApp();
  XCTAssert(!context, @"Context is NULL at setUp");
  theApp->make_current();
  context = new gl::MonoContext(theApp);
  
}

// After each test.
- (void)tearDown {
  [super tearDown];
  delete context;
  context = NULL;
  delete theApp;
}


/*
- (void)testExample {
  // This is an example of a functional test case.
  XCTAssert(YES, @"Pass");
}

- (void)testPerformanceExample {
  // This is an example of a performance test case.
  [self measureBlock:^{
      // Put the code you want to measure the time of here.
  }];
}
*/

- (void)testContextCurrent {
  glfwApp app2;
  XCTAssert(context->current(), @"Context is not current");
  
  gl::MonoContext context2(&app2);
  XCTAssert(context2.current(), @"Context 2 is not current");
  XCTAssert(!context->current(), @"Context is current when it shouldn't be");
}

- (void)testGLVersion {
  auto major = context->major_version();
  auto minor = context->minor_version();
  XCTAssert(major == 4u, @"GL major version is %u, expected 4", major);
  XCTAssert(minor == 1u, @"GL minor version is %u, expected 1", minor);
}

- (void)testShaders {
  try {
    gl::VertexShader vert ("shaders/vert.glsl");
    XCTAssert(vert.compiled(), @"Shader compilation failed");
  } catch(gl::exception const& e) {
    XCTAssert(false, @"Shader construction threw exception: %s", e.what());
  }
  
  EXPECT_THROW(gl::VertexShader bad_path("garbage"), @"Bad file path didn't fail");
  EXPECT_THROW(gl::VertexShader syntax_error_vert("shaders/bad_syntax.glsl"),
    @"Shader with bad syntax compiled");

  gl::VertexShader shader;
  shader.set_source({
    "hello", "cruel", "world",
  });
  XCTAssert(shader.get_source() == "hellocruelworld\n", @"shader set source doesn't match input");
  
}

- (void)testProgram {
  gl::VertexShader vert ("shaders/vert.glsl");
  gl::FragmentShader frag ("shaders/frag.glsl");
  gl::Program program (vert, frag);
  context->use(program);
  
  gl::Program program2 (*context, gl::VertexShader("shaders/vert.glsl"), gl::FragmentShader("shaders/frag.glsl"));
  
}

- (void)testUniforms {
  gl::VertexShader vert ("shaders/vert.glsl");
  gl::FragmentShader frag ("shaders/frag.glsl");
  gl::Program program (*context, vert, frag);
  
  {
    gl::uniform<int> a (program, program.uniform_location("blah"));
    XCTAssert(a.location() == -1, @"Non-existent uniform shouldn't be found");
  }
  
  {
    gl::uniform<int> a (program, program.uniform_location("color"));
    XCTAssert(a.location() != -1, @"Non-existent uniform 'color' should have be found");
  }
  
  {
    gl::uniform<int> a (program, program.uniform_location("color"));
    XCTAssert(a.location() != -1, @"Non-existent uniform 'color' should have be found");
    EXPECT_THROW(a.set(2), @"Expected throw when trying to set vec4 with a uniform<int>");
  }
  
  {
    gl::uniform4<float> color (program, program.uniform_location("color"));
    gl::vec4<float> val(0.1f, 0.2f, 0.3f, 0.4f);
    color.set(val);
    XCTAssert(color.get() == val, @"uniform 'color' should match value after set by vec4");
    color.set(0.1f, 0.2f, 0.3f, 0.4f);
    XCTAssert(color.get() == val, @"uniform 'color' should match value after set by 4 floats");
  }
}

- (void)testUniformMatrix {
  gl::VertexShader vert ("shaders/vert.glsl");
  gl::FragmentShader frag ("shaders/frag.glsl");
  gl::Program program (*context, vert, frag);

  glm::mat4 projection_matrix {
    glm::perspective<GLfloat>(
      -45,
      float(theApp->width()) / float(theApp->height()),
      1.0,
      2500.0
    )
  };
  
  {
    gl::uniform_mat4 p (program, program.uniform_location("projection"));
    p.set(glm::value_ptr(projection_matrix));
  }

}

- (void)testBufferMapping {
  gl::Buffer buffer;
  std::vector<float> v { 97.f, 98.f, 99.f, 100.f };
  buffer.data(v, GL_STATIC_READ);
  
  float* mapping = reinterpret_cast<float*>(buffer.map(GL_ARRAY_BUFFER, GL_READ_WRITE));
  
  EXPECT_THROW(buffer.map(GL_ARRAY_BUFFER, GL_READ_WRITE), @"re-mapping buffer should throw");
  
  XCTAssert(mapping[0] == 97.f, @"mapping[0] (%f) does not match input value!", mapping[0]);
  
  mapping[0] = 999.f;
  
  buffer.unmap();
  
  EXPECT_THROW(buffer.unmap(), @"re-unmapping unmapped buffer should throw");
  
  float o;
  buffer.get(0, sizeof(float), &o);
  XCTAssert(o == 999.f, @"output of subdata read (%f) should have matched mapping write", o);

}


- (void)testTextures {
  try {
    std::vector<uint32_t> pixels {
      0xff0000ff, 0x00ff00ff,
      0x000000ff, 0x0000ffff,
    };
    gl::ImageDesc2D desc (2, 2, pixels.data());
    gl::Texture2D t;
    t.image(0, desc);

    gl::ImageDesc2D desc2 (1, 2, pixels.data() + 1);
    t.subimage(0, 1, 0, desc2);
    
    gl::Buffer buffer;
    buffer.data(pixels, GL_STATIC_READ);
    t.unpack(0, buffer, desc, 0);
    
    t.copy(0, buffer, 0, 0, 2, 2);


  } catch(gl::exception const& e) {
    XCTAssert(false, @"exception: %s", e.what());
  }
  
}




@end
