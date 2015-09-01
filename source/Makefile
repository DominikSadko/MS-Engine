STANDARD_TYPE      = gnu++11
PLATFORM_TYPE      = 

IGNORE_FLAGS       = -Wall -Wextra -fpermissive -O0 -frtti


CXXFLAGS           = -march=i486
DEFINITIONS        =
INCLUDES           = -Ilibs/

LIBS               = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_image -lSDL2_net -lopengl32 -lbass -lPhysFS -mwindows

###########################################################################################################

MODULE_NAME        = MS-Engine Prototype!
RELEASE_DIRECTORY  = ../release
SOURCE_DIRECTORY   = ../source

ifeq ($(BUILD_TYPE),debug)
	RELEASE             = $(RELEASE_DIRECTORY)/debug.exe
	OBJECTS_DIRECTORY   = ../objects/debug
	CXXFLAGS		   +=  -g
else
	RELEASE             = $(RELEASE_DIRECTORY)/release.exe
	OBJECTS_DIRECTORY   = ../objects/release
	CXXFLAGS		   +=  
endif


###########################################################################################################

SOURCES       := 	$(SOURCE_DIRECTORY)/main.cpp \
	$(SOURCE_DIRECTORY)/framework/core/windows.cpp \
	$(SOURCE_DIRECTORY)/framework/core/platform.cpp \
	$(SOURCE_DIRECTORY)/framework/core/filemanager.cpp \
	$(SOURCE_DIRECTORY)/framework/core/filestream.cpp \
	$(SOURCE_DIRECTORY)/framework/core/resourcemanager.cpp \
	$(SOURCE_DIRECTORY)/framework/core/module.cpp \
	$(SOURCE_DIRECTORY)/framework/core/config.cpp \
	$(SOURCE_DIRECTORY)/framework/scheduler/scheduler.cpp \
	$(SOURCE_DIRECTORY)/framework/graphic/graphic.cpp \
	$(SOURCE_DIRECTORY)/framework/graphic/painter.cpp \
	$(SOURCE_DIRECTORY)/framework/graphic/shader.cpp \
	$(SOURCE_DIRECTORY)/framework/image/image.cpp \
	$(SOURCE_DIRECTORY)/framework/font/font.cpp \
	$(SOURCE_DIRECTORY)/framework/audio/audio.cpp \
	$(SOURCE_DIRECTORY)/framework/ui/uiwidget.cpp \
	$(SOURCE_DIRECTORY)/framework/ui/uitimer.cpp \
	$(SOURCE_DIRECTORY)/framework/ui/anchor.cpp \
	$(SOURCE_DIRECTORY)/framework/ui/uicondition.cpp \
	$(SOURCE_DIRECTORY)/framework/physic/uiscene.cpp \
	$(SOURCE_DIRECTORY)/framework/physic/body.cpp \
	$(SOURCE_DIRECTORY)/framework/physic/fixture.cpp \
	$(SOURCE_DIRECTORY)/framework/physic/joint.cpp \
	$(SOURCE_DIRECTORY)/framework/physic/camera.cpp \
	$(SOURCE_DIRECTORY)/framework/network/client.cpp \
	$(SOURCE_DIRECTORY)/framework/network/connection.cpp \
	$(SOURCE_DIRECTORY)/framework/lua/register.cpp \
	$(SOURCE_DIRECTORY)/framework/lua/parser.cpp \
	$(SOURCE_DIRECTORY)/framework/lua/utility.cpp \
									 \
	$(SOURCE_DIRECTORY)/libs/json/json11.cpp \
	$(SOURCE_DIRECTORY)/libs/stdex/format.cpp \
									 \
	$(SOURCE_DIRECTORY)/libs/lua52/lapi.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lauxlib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lbaselib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lbitlib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lcode.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lcorolib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lctype.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/ldblib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/ldebug.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/ldo.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/ldump.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lfunc.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lgc.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/linit.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/liolib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/llex.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lmathlib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lmem.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/loadlib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lobject.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lopcodes.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/loslib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lparser.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lstate.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lstring.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lstrlib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/ltable.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/ltablib.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/ltm.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lundump.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lvm.cpp \
	$(SOURCE_DIRECTORY)/libs/lua52/lzio.cpp \
	                                 \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/b2BroadPhase.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/b2CollideCircle.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/b2CollideEdge.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/b2CollidePolygon.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/b2Collision.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/b2Distance.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/b2DynamicTree.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/b2TimeOfImpact.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/Shapes/b2CircleShape.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/Shapes/b2EdgeShape.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/Shapes/b2ChainShape.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Collision/Shapes/b2PolygonShape.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Common/b2BlockAllocator.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Common/b2Draw.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Common/b2Math.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Common/b2Settings.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Common/b2StackAllocator.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Common/b2Timer.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/b2Body.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/b2ContactManager.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/b2Fixture.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/b2Island.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/b2World.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/b2WorldCallbacks.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Contacts/b2Contact.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2GearJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2Joint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
	$(SOURCE_DIRECTORY)/libs/Box2D/Dynamics/Joints/b2WheelJoint.cpp \

OBJECTS        = $(patsubst %.cpp, %.o, $(patsubst $(SOURCE_DIRECTORY)%,$(OBJECTS_DIRECTORY)%,$(SOURCES)))


CXXFLAGS      += $(IGNORE_FLAGS) -std=$(STANDARD_TYPE) $(PLATFORM_TYPE) -I. $(DEFINITIONS) $(INCLUDES)
DEFINITIONS   += -DMODULE_NAME="\"$(MODULE_NAME)\"" -DNDEBUG=1

all: $(OBJECTS)
	@if not exist $(RELEASE_DIRECTORY) mkdir $(RELEASE_DIRECTORY)

	@echo *****-----------------------------          Release: $<
	g++  $(OBJECTS) $(LIBS) $(DEFINITIONS) -o $(RELEASE)


$(OBJECTS): $(OBJECTS_DIRECTORY)/%.o : $(SOURCE_DIRECTORY)/%.cpp
	@echo *****-----------------------------          Building: $<

	@if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
	g++ $(LIBS) -c $(CXXFLAGS) $< -o $(patsubst %.cpp, %.o, $(patsubst $(SOURCE_DIRECTORY)%,$(OBJECTS_DIRECTORY)%,$<))

clean:
	rm $(OBJECTS)
	rm $(RELEASE)