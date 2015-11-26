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




@end
