#include "framework/ui/uiwidget.h"
#include "framework/ui/uitimer.h"
#include "framework/physic/uiscene.h"
#include "framework/physic/body.h"
#include "framework/physic/fixture.h"
#include "framework/physic/joint.h"

#include "framework/core/config.h"
#include "framework/core/filemanager.h"
#include "framework/core/resourcemanager.h"
#include "framework/scheduler/scheduler.h"

#include "framework/lua/lua.h"
#include <json/json11.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////

Lua::State g_lua;

void LuaRegister()
{
    g_lua.newMetatable("UIRoot");

    g_lua.newMetatable("Image");
    g_lua.constructor<Image, std::string>("Image.new");
    g_lua.destructor<Image>("Image.delete");
    g_lua.sharedFunction(&Image::getTexture,            "Image.getTexture");
    g_lua.sharedFunction(&Image::getWidth,              "Image.getWidth");
    g_lua.sharedFunction(&Image::getHeight,             "Image.getHeight");
    g_lua.sharedFunction(&Image::getTextureWidth,       "Image.getTextureWidth");
    g_lua.sharedFunction(&Image::getTextureHeight,      "Image.getTextureHeight");

    g_lua.newMetatable("Font");
    g_lua.constructor<Font, std::string, int32_t>("Font.new");
    g_lua.destructor<Font>("Font.delete");
    g_lua.sharedFunction(&Font::getTextSize,            "Font.getTextSize");
    g_lua.sharedFunction(&Font::setMask,                "Font.setMask");
    g_lua.sharedFunction(&Font::getMask,                "Font.getMask");

    g_lua.newMetatable("Shader");

    g_lua.newMetatable("UIWidget");
    g_lua.constructor<UIWidget>("UIWidget.new");
    g_lua.sharedFunction(&UIWidget::remove,             "UIWidget.delete");
    g_lua.sharedFunction(&UIWidget::isRemoved,          "UIWidget.isRemoved");

    /////////////////       Core       /////////////////
    g_lua.sharedFunction(&UIWidget::setId,              "UIWidget.setId");
    g_lua.sharedFunction(&UIWidget::setRect,            "UIWidget.setRect");
    g_lua.sharedFunction(&UIWidget::setPosition,        "UIWidget.setPosition");
    g_lua.sharedFunction(&UIWidget::setSize,            "UIWidget.setSize");
    g_lua.sharedFunction(&UIWidget::setX,               "UIWidget.setX");
    g_lua.sharedFunction(&UIWidget::setY,               "UIWidget.setY");
    g_lua.sharedFunction(&UIWidget::setWidth,           "UIWidget.setWidth");
    g_lua.sharedFunction(&UIWidget::setHeight,          "UIWidget.setHeight");
    g_lua.sharedFunction(&UIWidget::moveLeftBorder,     "UIWidget.moveLeftBorder");
    g_lua.sharedFunction(&UIWidget::moveTopBorder,      "UIWidget.moveTopBorder");
    g_lua.sharedFunction(&UIWidget::setPolygon,         "UIWidget.setPolygon");
    g_lua.sharedFunction(&UIWidget::setRotation,        "UIWidget.setRotation");
    g_lua.sharedFunction(&UIWidget::setOpacity,         "UIWidget.setOpacity");

    g_lua.sharedFunction(&UIWidget::getId,              "UIWidget.getId");
    g_lua.sharedFunction(&UIWidget::getUniqueId,        "UIWidget.getUniqueId");
    g_lua.sharedFunction(&UIWidget::getRect,            "UIWidget.getRect");
    g_lua.sharedFunction(&UIWidget::getVirtualRect,     "UIWidget.getVirtualRect");
    g_lua.sharedFunction(&UIWidget::getPosition,        "UIWidget.getPosition");
    g_lua.sharedFunction(&UIWidget::getCenterPosition,  "UIWidget.getCenterPosition");
    g_lua.sharedFunction(&UIWidget::getSize,            "UIWidget.getSize");
    g_lua.sharedFunction(&UIWidget::getX,               "UIWidget.getX");
    g_lua.sharedFunction(&UIWidget::getY,               "UIWidget.getY");
    g_lua.sharedFunction(&UIWidget::getWidth,           "UIWidget.getWidth");
    g_lua.sharedFunction(&UIWidget::getHeight,          "UIWidget.getHeight");
    g_lua.sharedFunction(&UIWidget::getRotation,        "UIWidget.getRotation");
    g_lua.sharedFunction(&UIWidget::getOpacity,         "UIWidget.getOpacity");

    g_lua.sharedFunction(&UIWidget::setVisible,         "UIWidget.setVisible");
    g_lua.sharedFunction(&UIWidget::setFocusable,       "UIWidget.setFocusable");
    g_lua.sharedFunction(&UIWidget::setMovable,         "UIWidget.setMovable");
    g_lua.sharedFunction(&UIWidget::setPhantom,         "UIWidget.setPhantom");
    g_lua.sharedFunction(&UIWidget::setSelected,        "UIWidget.setSelected");
    g_lua.sharedFunction(&UIWidget::setOn,              "UIWidget.setOn");
    g_lua.sharedFunction(&UIWidget::setLocked,          "UIWidget.setLocked");
    g_lua.sharedFunction(&UIWidget::setVirtualArea,     "UIWidget.setVirtualArea");
    g_lua.sharedFunction(&UIWidget::setTextAutoResize,  "UIWidget.setTextAutoResize");
    g_lua.sharedFunction(&UIWidget::setFixedRotation,   "UIWidget.setFixedRotation");
    g_lua.sharedFunction(&UIWidget::setCircle,          "UIWidget.setCircle");

    g_lua.sharedFunction(&UIWidget::isVisible,          "UIWidget.isVisible");
    g_lua.sharedFunction(&UIWidget::isFocusable,        "UIWidget.isFocusable");
    g_lua.sharedFunction(&UIWidget::isMovable,          "UIWidget.isMovable");
    g_lua.sharedFunction(&UIWidget::isPhantom,          "UIWidget.isPhantom");
    g_lua.sharedFunction(&UIWidget::isSelected,         "UIWidget.isSelected");
    g_lua.sharedFunction(&UIWidget::isOn,               "UIWidget.isOn");
    g_lua.sharedFunction(&UIWidget::isLocked,           "UIWidget.isLocked");
    g_lua.sharedFunction(&UIWidget::isVirtualArea,      "UIWidget.isVirtualArea");
    g_lua.sharedFunction(&UIWidget::isTextAutoResize,   "UIWidget.isTextAutoResize");
    g_lua.sharedFunction(&UIWidget::isFixedRotation,    "UIWidget.isFixedRotation");
    g_lua.sharedFunction(&UIWidget::isCircle,           "UIWidget.isCircle");

    g_lua.sharedFunction(&UIWidget::focus,              "UIWidget.focus");
    g_lua.sharedFunction(&UIWidget::grab,               "UIWidget.grab");

    g_lua.sharedFunction(&UIWidget::setMetatable,       "UIWidget.setMetatable");
    g_lua.sharedFunction(&UIWidget::getMetatable,       "UIWidget.getMetatable");

    /////////////////       Childs       ////////////////
    g_lua.sharedFunction(&UIWidget::doCreateChild,      "UIWidget.doCreateChild");
    g_lua.sharedFunction(&UIWidget::doCreateJsonChild,  "UIWidget.doCreateJsonChild");
    g_lua.sharedFunction(&UIWidget::setParent,          "UIWidget.setParent");
    g_lua.sharedFunction(&UIWidget::addChild,           "UIWidget.addChild");
    g_lua.sharedFunction(&UIWidget::eraseChild,         "UIWidget.eraseChild");
    g_lua.sharedFunction(&UIWidget::removeChilds,       "UIWidget.removeChilds");
    g_lua.sharedFunction(&UIWidget::removeChild,        "UIWidget.removeChild");
    g_lua.sharedFunction(&UIWidget::popChild,           "UIWidget.popChild");

    g_lua.sharedFunction(&UIWidget::hasChild,           "UIWidget.hasChild");
    g_lua.sharedFunction(&UIWidget::getChildById,       "UIWidget.getChildById");
    g_lua.sharedFunction(&UIWidget::getChildByRecursiveId,"UIWidget.getChildByRecursiveId");
    g_lua.sharedFunction(&UIWidget::getChildByPoint,    "UIWidget.getChildByPoint");
    g_lua.sharedFunction(&UIWidget::getChildByIndex,    "UIWidget.getChildByIndex");
    g_lua.sharedFunction(&UIWidget::getChildIndex,      "UIWidget.getChildIndex");
    g_lua.sharedFunction(&UIWidget::getChildsCount,     "UIWidget.getChildsCount");
    g_lua.sharedFunction(&UIWidget::getParent,          "UIWidget.getParent");
    g_lua.sharedFunction(&UIWidget::getParentByRecursiveId,"UIWidget.getParentByRecursiveId");

    /////////////////       Image       /////////////////
    g_lua.sharedFunction(&UIWidget::setImageSource,     "UIWidget.setImageSource");
    g_lua.sharedFunction(&UIWidget::setImageRect,       "UIWidget.setImageRect");
    g_lua.sharedFunction(&UIWidget::setImageRectX,      "UIWidget.setImageRectX");
    g_lua.sharedFunction(&UIWidget::setImageRectY,      "UIWidget.setImageRectY");
    g_lua.sharedFunction(&UIWidget::setImageRectW,      "UIWidget.setImageRectW");
    g_lua.sharedFunction(&UIWidget::setImageRectH,      "UIWidget.setImageRectH");
    g_lua.sharedFunction(&UIWidget::setImageColor,      "UIWidget.setImageColor");
    g_lua.sharedFunction(&UIWidget::setImageOpacity,    "UIWidget.setImageOpacity");
    g_lua.sharedFunction(&UIWidget::setImageCoords,     "UIWidget.setImageCoords");

    g_lua.sharedFunction(&UIWidget::getImageSource,     "UIWidget.getImageSource");
    g_lua.sharedFunction(&UIWidget::getImageRect,       "UIWidget.getImageRect");
    g_lua.sharedFunction(&UIWidget::getImageRectX,      "UIWidget.getImageRectX");
    g_lua.sharedFunction(&UIWidget::getImageRectY,      "UIWidget.getImageRectY");
    g_lua.sharedFunction(&UIWidget::getImageRectW,      "UIWidget.getImageRectW");
    g_lua.sharedFunction(&UIWidget::getImageRectH,      "UIWidget.getImageRectH");
    g_lua.sharedFunction(&UIWidget::getImageColor,      "UIWidget.getImageColor");
    g_lua.sharedFunction(&UIWidget::getImageOpacity,    "UIWidget.getImageOpacity");

    g_lua.sharedFunction(&UIWidget::setHorizontalMirror, "UIWidget.setHorizontalMirror");
    g_lua.sharedFunction(&UIWidget::setVerticalMirror,   "UIWidget.setVerticalMirror");
    g_lua.sharedFunction(&UIWidget::isHorizontalMirror,  "UIWidget.isHorizontalMirror");
    g_lua.sharedFunction(&UIWidget::isVerticalMirror,    "UIWidget.isVerticalMirror");

    /////////////////       Border       /////////////////
    g_lua.sharedFunction(&UIWidget::setBorder,          "UIWidget.setBorder");
    g_lua.sharedFunction(&UIWidget::setBorderLeft,      "UIWidget.setBorderLeft");
    g_lua.sharedFunction(&UIWidget::setBorderTop,       "UIWidget.setBorderTop");
    g_lua.sharedFunction(&UIWidget::setBorderRight,     "UIWidget.setBorderRight");
    g_lua.sharedFunction(&UIWidget::setBorderBottom,    "UIWidget.setBorderBottom");

    g_lua.sharedFunction(&UIWidget::getBorder,          "UIWidget.getBorder");
    g_lua.sharedFunction(&UIWidget::getBorderLeft,      "UIWidget.getBorderLeft");
    g_lua.sharedFunction(&UIWidget::getBorderTop,       "UIWidget.getBorderTop");
    g_lua.sharedFunction(&UIWidget::getBorderRight,     "UIWidget.getBorderRight");
    g_lua.sharedFunction(&UIWidget::getBorderBottom,    "UIWidget.getBorderBottom");

    /////////////////       Text       /////////////////
    g_lua.sharedFunction(&UIWidget::setText,            "UIWidget.setText");
    g_lua.sharedFunction(&UIWidget::setTextFont,        "UIWidget.setTextFont");
    g_lua.sharedFunction(&UIWidget::setTextPosition,    "UIWidget.setTextPosition");
    g_lua.sharedFunction(&UIWidget::setTextColor,       "UIWidget.setTextColor");
    g_lua.sharedFunction(&UIWidget::setTextOpacity,     "UIWidget.setTextOpacity");

    g_lua.sharedFunction(&UIWidget::getText,            "UIWidget.getText");
    g_lua.sharedFunction(&UIWidget::getTextFont,        "UIWidget.getTextFont");
    g_lua.sharedFunction(&UIWidget::getTextPosition,    "UIWidget.getTextPosition");
    g_lua.sharedFunction(&UIWidget::getTextColor,       "UIWidget.getTextColor");
    g_lua.sharedFunction(&UIWidget::getTextOpacity,     "UIWidget.getTextOpacity");

    /////////////////       Shader       /////////////////
    g_lua.sharedFunction(&UIWidget::setShader,          "UIWidget.setShader");
    g_lua.sharedFunction(&UIWidget::getShader,          "UIWidget.getShader");

    /////////////////       Anchor       /////////////////
    g_lua.sharedFunction(&UIWidget::setAnchor,          "UIWidget.setAnchor");
    g_lua.sharedFunction(&UIWidget::fill,               "UIWidget.fill");
    g_lua.sharedFunction(&UIWidget::removeAnchor,       "UIWidget.removeAnchor");
    g_lua.sharedFunction(&UIWidget::removeAnchors,      "UIWidget.removeAnchors");
    g_lua.sharedFunction(&UIWidget::setOffset,          "UIWidget.setOffset");
    g_lua.sharedFunction(&UIWidget::getOffset,          "UIWidget.getOffset");
    g_lua.sharedFunction(&UIWidget::setAnchorMargin,    "UIWidget.setAnchorMargin");
    g_lua.sharedFunction(&UIWidget::setAnchorMargins,   "UIWidget.setAnchorMargins");
    g_lua.sharedFunction(&UIWidget::getAnchorMargin,    "UIWidget.getAnchorMargin");
    g_lua.sharedFunction(&UIWidget::setAnchorScale,     "UIWidget.setAnchorScale");
    g_lua.sharedFunction(&UIWidget::getAnchorScale,     "UIWidget.getAnchorScale");

    g_lua.sharedFunction(&UIWidget::setResourceManager, "UIWidget.setResourceManager");
    g_lua.sharedFunction(&UIWidget::getResourceManager, "UIWidget.getResourceManager");
    g_lua.sharedFunction(&UIWidget::addLayout,          "UIWidget.addLayout");
    g_lua.sharedFunction(&UIWidget::removeCondition,    "UIWidget.removeCondition");

    g_lua.staticFunction(std::function<std::string(UIWidget*)>([]
            (UIWidget* widget)
    		{
				return widget->getId();
    		}), "UIWidget.tostring");

    g_lua.newMetatable("Condition");
    g_lua.constructor<UICondition, UIRoot*, uint8_t, std::function<bool()>*, uint16_t, uint16_t>("Condition.Custom");
    g_lua.staticFunction(&UICondition::FadeIn<UIRoot>,      "Condition.FadeIn");
    g_lua.staticFunction(&UICondition::FadeOut<UIRoot>,     "Condition.FadeOut");
    g_lua.staticFunction(&UICondition::Shine<UIRoot>,       "Condition.Shine");
    g_lua.staticFunction(&UICondition::Blink<UIRoot>,       "Condition.Blink");
    g_lua.staticFunction(&UICondition::Move<UIRoot>,        "Condition.Move");
    g_lua.staticFunction(&UICondition::Move<UIRoot>,        "Condition.MoveTo");
    g_lua.staticFunction(&UICondition::Acceleration,  		"Condition.Acceleration");
    g_lua.staticFunction(&UICondition::Graviton,     		"Condition.Graviton");

    std::map<std::string, uint8_t> conditionMap;
    conditionMap["None"]           = UIConditionType::None;
    conditionMap["FadeIn"]         = UIConditionType::FadeIn;
    conditionMap["FadeOut"]        = UIConditionType::FadeOut;
    conditionMap["Shine"]          = UIConditionType::Shine;
    conditionMap["Blink"]          = UIConditionType::Blink;
    conditionMap["Move"]           = UIConditionType::Move;
    conditionMap["MoveTo"]         = UIConditionType::MoveTo;
    conditionMap["Acceleration"]   = UIConditionType::Acceleration;
    conditionMap["Graviton"]       = UIConditionType::Graviton;
    g_lua.set("Condition.Type", conditionMap);

    /////////////////////////////////////////////////////////////////////////////////////////////

    g_lua.newMetatable("UITimer", "UIWidget");
    g_lua.constructor<UITimer>("UITimer.new");
    g_lua.constructor<UITimer>("UITimer.Create");
    g_lua.sharedFunction(&UITimer::remove,               "UITimer.delete");
    g_lua.sharedFunction(&UITimer::setStartTime,         "UITimer.setStartTime");
    g_lua.sharedFunction(&UITimer::getStartTime,         "UITimer.getStartTime");

    /////////////////////////////////////////////////////////////////////////////////////////////

    g_lua.newMetatable("UIScene", "UIWidget");
    g_lua.constructor<UIScene>("UIScene.new");
    g_lua.sharedFunction(&UIScene::remove,                "UIScene.delete");
    g_lua.sharedFunction(&UIScene::init,                  "UIScene.init");
    g_lua.sharedFunction(&UIScene::terminate,             "UIScene.terminate");
    g_lua.sharedFunction(&UIScene::sleep,                 "UIScene.sleep");
    g_lua.sharedFunction(&UIScene::awake,                 "UIScene.awake");
    g_lua.sharedFunction(&UIScene::isSleeping,            "UIScene.isSleeping");
    g_lua.sharedFunction(&UIScene::setStep,               "UIScene.setStep");
    g_lua.sharedFunction(&UIScene::getTimeStep,           "UIScene.getTimeStep");
    g_lua.sharedFunction(&UIScene::getVelocityIterations, "UIScene.getVelocityIterations");
    g_lua.sharedFunction(&UIScene::getPositionIterations, "UIScene.getPositionIterations");
    g_lua.sharedFunction(&UIScene::setScale,              "UIScene.setScale");
    g_lua.sharedFunction(&UIScene::getScale,              "UIScene.getScale");

    g_lua.sharedFunction(&UIScene::getFirstBody,          "UIScene.getFirstBody");
    g_lua.sharedFunction(&UIScene::removeBody,            "UIScene.removeBody");
    g_lua.sharedFunction(&UIScene::removeJoint,           "UIScene.removeJoint");
    g_lua.sharedFunction(&UIScene::getBodyCount,          "UIScene.getBodyCount");
    g_lua.sharedFunction(&UIScene::getJointCount,         "UIScene.getJointCount");
    g_lua.sharedFunction(&UIScene::setGravity,            "UIScene.setGravity");
    g_lua.sharedFunction(&UIScene::getGravity,            "UIScene.getGravity");
    g_lua.sharedFunction(&UIScene::queryAABB,             "UIScene.queryAABB");
    g_lua.sharedFunction(&UIScene::rayCast,               "UIScene.rayCast");
    g_lua.sharedFunction(&UIScene::getFixtureList,        "UIScene.getFixtureList");
    g_lua.sharedFunction(&UIScene::getBodyList,           "UIScene.getBodyList");
    g_lua.sharedFunction(&UIScene::setCamera,             "UIScene.setCamera");
    g_lua.sharedFunction(&UIScene::getCamera,             "UIScene.getCamera");
    g_lua.sharedFunction(&UIScene::setCameraOffset,       "UIScene.setCameraOffset");
    g_lua.sharedFunction(&UIScene::getCameraOffset,       "UIScene.getCameraOffset");

    g_lua.newMetatable("Body");
    g_lua.constructor<Body, UIScene*>("Body.new");
    g_lua.sharedFunction(&Body::setMetatable,          "Body.setMetatable");
    g_lua.sharedFunction(&Body::getMetatable,          "Body.getMetatable");

    g_lua.sharedFunction(&Body::setImage,              "Body.setImage");
    g_lua.sharedFunction(&Body::getImage,              "Body.getImage");
    g_lua.sharedFunction(&Body::removeFixture,         "Body.removeFixture");
    g_lua.sharedFunction(&Body::getFixture,            "Body.getFixture");
    g_lua.sharedFunction(&Body::getNext,               "Body.getNext");

    g_lua.sharedFunction(&Body::setType,               "Body.setType");
    g_lua.sharedFunction(&Body::setPosition,           "Body.setPosition");
    g_lua.sharedFunction(&Body::setRotation,           "Body.setRotation");
    g_lua.sharedFunction(&Body::setLinearVelocity,     "Body.setLinearVelocity");
    g_lua.sharedFunction(&Body::setAngularVelocity,    "Body.setAngularVelocity");
    g_lua.sharedFunction(&Body::setGravityScale,       "Body.setGravityScale");
    g_lua.sharedFunction(&Body::setLinearDamping,      "Body.setLinearDamping");
    g_lua.sharedFunction(&Body::setAngularDamping,     "Body.setAngularDamping");
    g_lua.sharedFunction(&Body::setBullet,             "Body.setBullet");
    g_lua.sharedFunction(&Body::setFixedRotation,      "Body.setFixedRotation");
    g_lua.sharedFunction(&Body::getScene,              "Body.getScene");
    g_lua.sharedFunction(&Body::getType,               "Body.getType");
    g_lua.sharedFunction(&Body::getMass,               "Body.getMass");
    g_lua.sharedFunction(&Body::getInertia,            "Body.getInertia");
    g_lua.sharedFunction(&Body::getPosition,           "Body.getPosition");
    g_lua.sharedFunction(&Body::getRotation,           "Body.getRotation");
    g_lua.sharedFunction(&Body::getLinearVelocity,     "Body.getLinearVelocity");
    g_lua.sharedFunction(&Body::getAngularVelocity,    "Body.getAngularVelocity");
    g_lua.sharedFunction(&Body::getGravityScale,       "Body.getGravityScale");
    g_lua.sharedFunction(&Body::getLinearDamping,      "Body.getLinearDamping");
    g_lua.sharedFunction(&Body::getAngularDamping,     "Body.getAngularDamping");
    g_lua.sharedFunction(&Body::isBullet,              "Body.isBullet");
    g_lua.sharedFunction(&Body::isFixedRotation,       "Body.isFixedRotation");
    g_lua.sharedFunction(&Body::resetMassData,         "Body.resetMassData");
    g_lua.sharedFunction(&Body::applyForce,            "Body.applyForce");
    g_lua.sharedFunction(&Body::applyForceToCenter,    "Body.applyForceToCenter");
    g_lua.sharedFunction(&Body::applyTorque,           "Body.applyTorque");
    g_lua.sharedFunction(&Body::applyLinearImpulse,    "Body.applyLinearImpulse");
    g_lua.sharedFunction(&Body::applyAngularImpulse,   "Body.applyAngularImpulse");

    g_lua.sharedFunction(&Body::setColor,              "Body.setColor");
    g_lua.sharedFunction(&Body::getColor,              "Body.getColor");
    g_lua.sharedFunction(&Body::setOpacity,            "Body.setOpacity");
    g_lua.sharedFunction(&Body::getOpacity,            "Body.getOpacity");
    g_lua.sharedFunction(&Body::setVerticalMirror,     "Body.setVerticalMirror");
    g_lua.sharedFunction(&Body::isHorizontalMirror,    "Body.isHorizontalMirror");
    g_lua.sharedFunction(&Body::setHorizontalMirror,   "Body.setHorizontalMirror");
    g_lua.sharedFunction(&Body::isVerticalMirror,      "Body.isVerticalMirror");
    g_lua.sharedFunction(&Body::setShader,             "Body.setShader");
    g_lua.sharedFunction(&Body::getShader,             "Body.getShader");

    g_lua.newMetatable("Fixture");
    g_lua.sharedFunction(&Fixture::setMetatable,       "Fixture.setMetatable");
    g_lua.sharedFunction(&Fixture::getMetatable,       "Fixture.getMetatable");

    g_lua.staticFunction(&Fixture::Vertice,            "Fixture.Vertice");
    g_lua.staticFunction(&Fixture::Circle,             "Fixture.Circle");
    g_lua.staticFunction(&Fixture::Edge,               "Fixture.Edge");
    g_lua.staticFunction(&Fixture::Square,             "Fixture.Square");

    g_lua.sharedFunction(&Fixture::getNext,            "Fixture.getNext");
    g_lua.sharedFunction(&Fixture::getBody,            "Fixture.getBody");
    g_lua.sharedFunction(&Fixture::getType,            "Fixture.getType");
    g_lua.sharedFunction(&Fixture::isSensor,           "Fixture.isSensor");
    g_lua.sharedFunction(&Fixture::getDensity,         "Fixture.getDensity");
    g_lua.sharedFunction(&Fixture::getFriction,        "Fixture.getFriction");
    g_lua.sharedFunction(&Fixture::getRestitution,     "Fixture.getRestitution");
    g_lua.sharedFunction(&Fixture::setFilterData,      "Fixture.setFilterData");
    g_lua.sharedFunction(&Fixture::setSensor,          "Fixture.setSensor");
    g_lua.sharedFunction(&Fixture::setDensity,         "Fixture.setDensity");
    g_lua.sharedFunction(&Fixture::setFriction,        "Fixture.setFriction");
    g_lua.sharedFunction(&Fixture::setRestitution,     "Fixture.setRestitution");
    g_lua.sharedFunction(&Fixture::setPolygonVertices, "Fixture.setPolygonVertices");
    g_lua.sharedFunction(&Fixture::setCircleSize,      "Fixture.setCircleSize");
    g_lua.sharedFunction(&Fixture::setEdgeSize,        "Fixture.setEdgeSize");
    g_lua.sharedFunction(&Fixture::setSquareSize,      "Fixture.setSquareSize");
    g_lua.sharedFunction(&Fixture::addImageCoords,     "Fixture.addImageCoords");
    g_lua.sharedFunction(&Fixture::setImageCoords,     "Fixture.setImageCoords");
    g_lua.sharedFunction(&Fixture::updateImageCoords,  "Fixture.updateImageCoords");
    g_lua.sharedFunction(&Fixture::eraseImageCoords,   "Fixture.eraseImageCoords");
    g_lua.sharedFunction(&Fixture::addPattern,         "Fixture.addPattern");
    g_lua.sharedFunction(&Fixture::setPattern,         "Fixture.setPattern");

    g_lua.sharedFunction(&Fixture::setPhantom,         "Fixture.setPhantom");
    g_lua.sharedFunction(&Fixture::isPhantom,          "Fixture.isPhantom");

    g_lua.newMetatable("Joint");
    g_lua.staticFunction(&Joint::Revolute,             "Joint.Revolute");
    g_lua.staticFunction(&Joint::Distance,             "Joint.Distance");
    g_lua.staticFunction(&Joint::Prismatic,            "Joint.Prismatic");
    g_lua.staticFunction(&Joint::Friction,             "Joint.Friction");
    g_lua.staticFunction(&Joint::Gear,                 "Joint.Gear");
    g_lua.staticFunction(&Joint::Motor,                "Joint.Motor");
    g_lua.staticFunction(&Joint::Mouse,                "Joint.Mouse");
    g_lua.staticFunction(&Joint::Pulley,               "Joint.Pulley");
    g_lua.staticFunction(&Joint::Rope,                 "Joint.Rope");
    g_lua.staticFunction(&Joint::Weld,                 "Joint.Weld");
    g_lua.staticFunction(&Joint::Wheel,                "Joint.Wheel");

    g_lua.newMetatable("b2Joint");
    g_lua.sharedFunction(&b2Joint::GetType,                       "b2Joint.getType");
    g_lua.sharedFunction(&b2Joint::IsActive,                      "b2Joint.isActive");
    g_lua.sharedFunction(&b2Joint::GetCollideConnected,           "b2Joint.getCollideConnected");

    g_lua.newMetatable("RevoluteJoint", "b2Joint");
    g_lua.sharedFunction(&RevoluteJoint::GetReactionForce,      "RevoluteJoint.getReactionForce");
    g_lua.sharedFunction(&RevoluteJoint::GetReactionTorque,     "RevoluteJoint.getReactionTorque");
    g_lua.sharedFunction(&RevoluteJoint::GetLocalAnchorA,       "RevoluteJoint.getLocalAnchorA");
    g_lua.sharedFunction(&RevoluteJoint::GetLocalAnchorB,       "RevoluteJoint.getLocalAnchorB");
    g_lua.sharedFunction(&RevoluteJoint::GetReferenceAngle,     "RevoluteJoint.GetReferenceAngle");
    g_lua.sharedFunction(&RevoluteJoint::GetJointAngle,         "RevoluteJoint.getJointAngle");
    g_lua.sharedFunction(&RevoluteJoint::GetJointSpeed,         "RevoluteJoint.getJointSpeed");
    g_lua.sharedFunction(&RevoluteJoint::IsLimitEnabled,        "RevoluteJoint.isLimitEnabled");
    g_lua.sharedFunction(&RevoluteJoint::EnableLimit,           "RevoluteJoint.enableLimit");
    g_lua.sharedFunction(&RevoluteJoint::GetLowerLimit,         "RevoluteJoint.getLowerLimit");
    g_lua.sharedFunction(&RevoluteJoint::GetUpperLimit,         "RevoluteJoint.getUpperLimit");
    g_lua.sharedFunction(&RevoluteJoint::IsMotorEnabled,        "RevoluteJoint.isMotorEnabled");
    g_lua.sharedFunction(&RevoluteJoint::EnableMotor,           "RevoluteJoint.enableMotor");
    g_lua.sharedFunction(&RevoluteJoint::SetMotorSpeed,         "RevoluteJoint.setMotorSpeed");
    g_lua.sharedFunction(&RevoluteJoint::GetMotorSpeed,         "RevoluteJoint.getMotorSpeed");
    g_lua.sharedFunction(&RevoluteJoint::SetMaxMotorTorque,     "RevoluteJoint.setMaxMotorTorque");
    g_lua.sharedFunction(&RevoluteJoint::GetMaxMotorTorque,     "RevoluteJoint.getMaxMotorTorque");
    g_lua.sharedFunction(&RevoluteJoint::GetMotorTorque,        "RevoluteJoint.getMotorTorque");

    g_lua.newMetatable("DistanceJoint", "b2Joint");
    g_lua.sharedFunction(&DistanceJoint::GetReactionForce,       "DistanceJoint.getReactionForce");
    g_lua.sharedFunction(&DistanceJoint::GetReactionTorque,      "DistanceJoint.getReactionTorque");
    g_lua.sharedFunction(&DistanceJoint::GetLocalAnchorA,        "DistanceJoint.getLocalAnchorA");
    g_lua.sharedFunction(&DistanceJoint::GetLocalAnchorB,        "DistanceJoint.getLocalAnchorB");
    g_lua.sharedFunction(&DistanceJoint::SetLength,              "DistanceJoint.setLength");
    g_lua.sharedFunction(&DistanceJoint::GetLength,              "DistanceJoint.getLength");
    g_lua.sharedFunction(&DistanceJoint::SetFrequency,           "DistanceJoint.setFrequency");
    g_lua.sharedFunction(&DistanceJoint::SetDampingRatio,        "DistanceJoint.setDampingRatio");
    g_lua.sharedFunction(&DistanceJoint::GetDampingRatio,        "DistanceJoint.getDampingRatio");

    g_lua.newMetatable("PrismaticJoint", "b2Joint");
    g_lua.sharedFunction(&PrismaticJoint::GetReactionForce,      "PrismaticJoint.getReactionForce");
    g_lua.sharedFunction(&PrismaticJoint::GetReactionTorque,     "PrismaticJoint.getReactionTorque");
    g_lua.sharedFunction(&PrismaticJoint::GetLocalAnchorA,       "PrismaticJoint.getLocalAnchorA");
    g_lua.sharedFunction(&PrismaticJoint::GetLocalAnchorB,       "PrismaticJoint.getLocalAnchorB");
    g_lua.sharedFunction(&PrismaticJoint::GetLocalAxisA,         "PrismaticJoint.getLocalAxisA");
    g_lua.sharedFunction(&PrismaticJoint::GetReferenceAngle,     "PrismaticJoint.getReferenceAngle");
    g_lua.sharedFunction(&PrismaticJoint::GetJointTranslation,   "PrismaticJoint.getJointTranslation");
    g_lua.sharedFunction(&PrismaticJoint::GetJointSpeed,         "PrismaticJoint.getJointSpeed");
    g_lua.sharedFunction(&PrismaticJoint::IsLimitEnabled,        "PrismaticJoint.isLimitEnabled");
    g_lua.sharedFunction(&PrismaticJoint::EnableLimit,           "PrismaticJoint.enableLimit");
    g_lua.sharedFunction(&PrismaticJoint::GetLowerLimit,         "PrismaticJoint.getLowerLimit");
    g_lua.sharedFunction(&PrismaticJoint::GetUpperLimit,         "PrismaticJoint.getUpperLimit");
    g_lua.sharedFunction(&PrismaticJoint::SetLimits,             "PrismaticJoint.setLimits");
    g_lua.sharedFunction(&PrismaticJoint::IsMotorEnabled,        "PrismaticJoint.isMotorEnabled");
    g_lua.sharedFunction(&PrismaticJoint::EnableMotor,           "PrismaticJoint.enableMotor");
    g_lua.sharedFunction(&PrismaticJoint::SetMotorSpeed,         "PrismaticJoint.setMotorSpeed");
    g_lua.sharedFunction(&PrismaticJoint::GetMotorSpeed,         "PrismaticJoint.getMotorSpeed");
    g_lua.sharedFunction(&PrismaticJoint::SetMaxMotorForce,      "PrismaticJoint.setMaxMotorForce");
    g_lua.sharedFunction(&PrismaticJoint::GetMaxMotorForce,      "PrismaticJoint.getMaxMotorForce");
    g_lua.sharedFunction(&PrismaticJoint::GetMotorForce,         "PrismaticJoint.getMotorForce");

    g_lua.newMetatable("FrictionJoint", "b2Joint");
    g_lua.sharedFunction(&FrictionJoint::GetReactionForce,        "FrictionJoint.getReactionForce");
    g_lua.sharedFunction(&FrictionJoint::GetReactionTorque,       "FrictionJoint.getReactionTorque");
    g_lua.sharedFunction(&FrictionJoint::GetLocalAnchorA,         "FrictionJoint.getLocalAnchorA");
    g_lua.sharedFunction(&FrictionJoint::GetLocalAnchorB,         "FrictionJoint.getLocalAnchorB");
    g_lua.sharedFunction(&FrictionJoint::SetMaxForce,             "FrictionJoint.setMaxForce");
    g_lua.sharedFunction(&FrictionJoint::GetMaxForce,             "FrictionJoint.getMaxForce");
    g_lua.sharedFunction(&FrictionJoint::SetMaxTorque,            "FrictionJoint.setMaxTorque");
    g_lua.sharedFunction(&FrictionJoint::GetMaxTorque,            "FrictionJoint.getMaxTorque");

    g_lua.newMetatable("GearJoint", "b2Joint");
    g_lua.sharedFunction(&GearJoint::GetReactionForce,            "GearJoint.getReactionForce");
    g_lua.sharedFunction(&GearJoint::GetReactionTorque,           "GearJoint.getReactionTorque");
    g_lua.sharedFunction(&GearJoint::GetJoint1,                   "GearJoint.getJoint1");
    g_lua.sharedFunction(&GearJoint::GetJoint2,                   "GearJoint.getJoint2");
    g_lua.sharedFunction(&GearJoint::SetRatio,                    "GearJoint.setRatio");
    g_lua.sharedFunction(&GearJoint::GetRatio,                    "GearJoint.getRatio");

    g_lua.newMetatable("MotorJoint", "b2Joint");
    g_lua.sharedFunction(&MotorJoint::GetReactionForce,           "MotorJoint.getReactionForce");
    g_lua.sharedFunction(&MotorJoint::GetReactionTorque,          "MotorJoint.getReactionTorque");
    g_lua.sharedFunction(&MotorJoint::SetLinearOffset,            "MotorJoint.setLinearOffset");
    g_lua.sharedFunction(&MotorJoint::GetLinearOffset,            "MotorJoint.getLinearOffset");
    g_lua.sharedFunction(&MotorJoint::SetAngularOffset,           "MotorJoint.setAngularOffset");
    g_lua.sharedFunction(&MotorJoint::GetAngularOffset,           "MotorJoint.getAngularOffset");
    g_lua.sharedFunction(&MotorJoint::SetMaxForce,                "MotorJoint.setMaxForce");
    g_lua.sharedFunction(&MotorJoint::GetMaxForce,                "MotorJoint.getMaxForce");
    g_lua.sharedFunction(&MotorJoint::SetMaxTorque,               "MotorJoint.setMaxTorque");
    g_lua.sharedFunction(&MotorJoint::GetMaxTorque,               "MotorJoint.getMaxTorque");
    g_lua.sharedFunction(&MotorJoint::SetCorrectionFactor,        "MotorJoint.setCorrectionFactor");
    g_lua.sharedFunction(&MotorJoint::GetCorrectionFactor,        "MotorJoint.getCorrectionFactor");
    g_lua.sharedFunction(&MotorJoint::GetReactionForce,           "MotorJoint.getReactionForce");
    g_lua.sharedFunction(&MotorJoint::GetReactionForce,           "MotorJoint.getReactionForce");
    g_lua.sharedFunction(&MotorJoint::GetReactionForce,           "MotorJoint.getReactionForce");

    g_lua.newMetatable("MouseJoint", "b2Joint");
    g_lua.sharedFunction(&MouseJoint::GetReactionForce,           "MouseJoint.getReactionForce");
    g_lua.sharedFunction(&MouseJoint::GetReactionTorque,          "MouseJoint.getReactionTorque");
    g_lua.sharedFunction(&MouseJoint::SetTarget,                  "MouseJoint.setTarget");
    g_lua.sharedFunction(&MouseJoint::GetTarget,                  "MouseJoint.getTarget");
    g_lua.sharedFunction(&MouseJoint::SetMaxForce,                "MouseJoint.setMaxForce");
    g_lua.sharedFunction(&MouseJoint::GetMaxForce,                "MouseJoint.getMaxForce");
    g_lua.sharedFunction(&MouseJoint::SetFrequency,               "MouseJoint.setFrequency");
    g_lua.sharedFunction(&MouseJoint::GetFrequency,               "MouseJoint.getFrequency");
    g_lua.sharedFunction(&MouseJoint::SetDampingRatio,            "MouseJoint.setDampingRatio");
    g_lua.sharedFunction(&MouseJoint::GetDampingRatio,            "MouseJoint.getDampingRatio");
    g_lua.sharedFunction(&MouseJoint::ShiftOrigin,                "MouseJoint.shiftOrigin");

    g_lua.newMetatable("PulleyJoint", "b2Joint");
    g_lua.sharedFunction(&PulleyJoint::GetReactionForce,          "PulleyJoint.getReactionForce");
    g_lua.sharedFunction(&PulleyJoint::GetReactionTorque,         "PulleyJoint.getReactionTorque");
    g_lua.sharedFunction(&PulleyJoint::GetGroundAnchorA,          "PulleyJoint.getGroundAnchorA");
    g_lua.sharedFunction(&PulleyJoint::GetGroundAnchorB,          "PulleyJoint.getGroundAnchorB");
    g_lua.sharedFunction(&PulleyJoint::GetLengthA,                "PulleyJoint.getLengthA");
    g_lua.sharedFunction(&PulleyJoint::GetLengthB,                "PulleyJoint.getLengthB");
    g_lua.sharedFunction(&PulleyJoint::GetRatio,                  "PulleyJoint.getRatio");
    g_lua.sharedFunction(&PulleyJoint::GetCurrentLengthA,         "PulleyJoint.getCurrentLengthA");
    g_lua.sharedFunction(&PulleyJoint::GetCurrentLengthB,         "PulleyJoint.getCurrentLengthB");
    g_lua.sharedFunction(&PulleyJoint::ShiftOrigin,               "PulleyJoint.shiftOrigin");

    g_lua.newMetatable("RopeJoint", "b2Joint");
    g_lua.sharedFunction(&RopeJoint::GetReactionForce,            "RopeJoint.getReactionForce");
    g_lua.sharedFunction(&RopeJoint::GetReactionTorque,           "RopeJoint.getReactionTorque");
    g_lua.sharedFunction(&RopeJoint::GetLocalAnchorA,             "RopeJoint.getLocalAnchorA");
    g_lua.sharedFunction(&RopeJoint::GetLocalAnchorB,             "RopeJoint.getLocalAnchorB");
    g_lua.sharedFunction(&RopeJoint::SetMaxLength,                "RopeJoint.setMaxLength");
    g_lua.sharedFunction(&RopeJoint::GetMaxLength,                "RopeJoint.getMaxLength");
    g_lua.sharedFunction(&RopeJoint::GetLimitState,               "RopeJoint.getLimitState");

    g_lua.newMetatable("WeldJoint", "b2Joint");
    g_lua.sharedFunction(&WeldJoint::GetReactionForce,            "WeldJoint.getReactionForce");
    g_lua.sharedFunction(&WeldJoint::GetReactionTorque,           "WeldJoint.getReactionTorque");
    g_lua.sharedFunction(&WeldJoint::GetLocalAnchorA,             "WeldJoint.getLocalAnchorA");
    g_lua.sharedFunction(&WeldJoint::GetLocalAnchorB,             "WeldJoint.getLocalAnchorB");
    g_lua.sharedFunction(&WeldJoint::GetReferenceAngle,           "WeldJoint.getReferenceAngle");
    g_lua.sharedFunction(&WeldJoint::SetFrequency,                "WeldJoint.setFrequency");
    g_lua.sharedFunction(&WeldJoint::GetFrequency,                "WeldJoint.getFrequency");
    g_lua.sharedFunction(&WeldJoint::SetDampingRatio,             "WeldJoint.setDampingRatio");
    g_lua.sharedFunction(&WeldJoint::GetDampingRatio,             "WeldJoint.getDampingRatio");

    g_lua.newMetatable("b2WheelJoint", "b2Joint");
    g_lua.sharedFunction(&b2WheelJoint::GetReactionForce,           "WeldJoint.getReactionForce");
    g_lua.sharedFunction(&b2WheelJoint::GetReactionTorque,          "WeldJoint.getReactionTorque");
    g_lua.sharedFunction(&b2WheelJoint::GetLocalAnchorA,            "WeldJoint.getLocalAnchorA");
    g_lua.sharedFunction(&b2WheelJoint::GetLocalAnchorB,            "WeldJoint.getLocalAnchorB");
    g_lua.sharedFunction(&b2WheelJoint::GetLocalAxisA,              "WeldJoint.getLocalAxisA");
    g_lua.sharedFunction(&b2WheelJoint::GetJointTranslation,        "WeldJoint.getJointTranslation");
    g_lua.sharedFunction(&b2WheelJoint::GetJointSpeed,              "WeldJoint.getJointSpeed");
    g_lua.sharedFunction(&b2WheelJoint::IsMotorEnabled,             "WeldJoint.isMotorEnabled");
    g_lua.sharedFunction(&b2WheelJoint::EnableMotor,                "WeldJoint.enableMotor");
    g_lua.sharedFunction(&b2WheelJoint::SetMotorSpeed,              "WeldJoint.setMotorSpeed");
    g_lua.sharedFunction(&b2WheelJoint::GetMotorSpeed,              "WeldJoint.getMotorSpeed");
    g_lua.sharedFunction(&b2WheelJoint::SetMaxMotorTorque,          "WeldJoint.setMaxMotorTorque");
    g_lua.sharedFunction(&b2WheelJoint::GetMaxMotorTorque,          "WeldJoint.getMaxMotorTorque");
    g_lua.sharedFunction(&b2WheelJoint::GetMotorTorque,             "WeldJoint.getMotorTorque");
    g_lua.sharedFunction(&b2WheelJoint::SetSpringFrequencyHz,       "WeldJoint.setSpringFrequencyHz");
    g_lua.sharedFunction(&b2WheelJoint::GetSpringFrequencyHz,       "WeldJoint.getSpringFrequencyHz");
    g_lua.sharedFunction(&b2WheelJoint::SetSpringDampingRatio,      "WeldJoint.setSpringDampingRatio");
    g_lua.sharedFunction(&b2WheelJoint::GetSpringDampingRatio,      "WeldJoint.getSpringDampingRatio");

    /////////////////////////////////////////////////////////////////////////////////////////////

    g_lua.newMetatable("ResourceManager");
    g_lua.constructor<ResourceManager>("ResourceManager.new");
    g_lua.destructor<ResourceManager>("ResourceManager.delete");
    g_lua.sharedFunction(&ResourceManager::clear,        "ResourceManager.clear");
    g_lua.sharedFunction(&ResourceManager::addImage,     "ResourceManager.addImage");
    g_lua.sharedFunction(&ResourceManager::addFont,      "ResourceManager.addFont");
    g_lua.sharedFunction(&ResourceManager::addShader,    "ResourceManager.addShader");
    g_lua.sharedFunction(&ResourceManager::addLayout,    "ResourceManager.addLayout");
    g_lua.sharedFunction(&ResourceManager::addModule,    "ResourceManager.addModule");

    g_lua.sharedFunction(&ResourceManager::getImage,     "ResourceManager.getImage");
    g_lua.sharedFunction(&ResourceManager::getFont,      "ResourceManager.getFont");
    g_lua.sharedFunction(&ResourceManager::getShader,    "ResourceManager.getShader");
    g_lua.sharedFunction(&ResourceManager::getLayout,    "ResourceManager.getLayout");

    g_lua.sharedFunction(&ResourceManager::removeImage,  "ResourceManager.removeImage");
    g_lua.sharedFunction(&ResourceManager::removeFont,   "ResourceManager.removeFont");
    g_lua.sharedFunction(&ResourceManager::removeShader, "ResourceManager.removeShader");
    g_lua.sharedFunction(&ResourceManager::removeLayout, "ResourceManager.removeLayout");
    g_lua.sharedFunction(&ResourceManager::removeModule, "ResourceManager.removeModule");

    g_lua.newMetatable("AudioManager");
    g_lua.constructor<AudioManager>("AudioManager.new");
    g_lua.destructor<AudioManager>("AudioManager.delete");
    g_lua.sharedFunction(&AudioManager::init,          "AudioManager.init");
    g_lua.sharedFunction(&AudioManager::terminate,     "AudioManager.terminate");

    g_lua.sharedFunction(&AudioManager::add,           "AudioManager.add");
    g_lua.sharedFunction(&AudioManager::play,          "AudioManager.play");
    g_lua.sharedFunction(&AudioManager::pause,         "AudioManager.pause");
    g_lua.sharedFunction(&AudioManager::stop,          "AudioManager.stop");
    g_lua.sharedFunction(&AudioManager::remove,        "AudioManager.remove");
    g_lua.sharedFunction(&AudioManager::getPosition,   "AudioManager.getPosition");
    g_lua.sharedFunction(&AudioManager::setPosition,   "AudioManager.setPosition");
    g_lua.sharedFunction(&AudioManager::getLength,     "AudioManager.getLength");
    g_lua.sharedFunction(&AudioManager::getSeconds,    "AudioManager.getSeconds");
    g_lua.sharedFunction(&AudioManager::getArtist,     "AudioManager.getArtist");
    g_lua.sharedFunction(&AudioManager::getTitle,      "AudioManager.getTitle");
    g_lua.sharedFunction(&AudioManager::getAlbum,      "AudioManager.getAlbum");
    g_lua.sharedFunction(&AudioManager::hasTagCard,    "AudioManager.hasTagCard");

    g_lua.sharedFunction(&AudioManager::Mute,          "AudioManager.Mute");
    g_lua.sharedFunction(&AudioManager::Pause,         "AudioManager.Pause");
    g_lua.sharedFunction(&AudioManager::Resume,        "AudioManager.Resume");
    g_lua.sharedFunction(&AudioManager::SetVolume,     "AudioManager.SetVolume");
    g_lua.sharedFunction(&AudioManager::GetVolume,     "AudioManager.GetVolume");
    g_lua.sharedFunction(&AudioManager::SetMuted,      "AudioManager.SetMuted");
    g_lua.sharedFunction(&AudioManager::IsMuted,       "AudioManager.IsMuted");

    g_lua.newMetatable("g_fileManager");
    g_lua.instanceFunction(&FileManager::addSearchPath,     &g_fileManager, "g_fileManager.addSearchPath");
    g_lua.instanceFunction(&FileManager::removeSearchPath,  &g_fileManager, "g_fileManager.removeSearchPath");
    g_lua.instanceFunction(&FileManager::getUserDir,        &g_fileManager, "g_fileManager.getUserDir");
    g_lua.instanceFunction(&FileManager::fileExists,        &g_fileManager, "g_fileManager.fileExists");
    g_lua.instanceFunction(&FileManager::directoryExists,   &g_fileManager, "g_fileManager.directoryExists");
    g_lua.instanceFunction(&FileManager::resolvePath,       &g_fileManager, "g_fileManager.resolvePath");
    g_lua.instanceFunction(&FileManager::getExtension,      &g_fileManager, "g_fileManager.getExtension");
    g_lua.instanceFunction(&FileManager::getFileName,       &g_fileManager, "g_fileManager.getFileName");
    g_lua.instanceFunction(&FileManager::getDirectoryFiles, &g_fileManager, "g_fileManager.getDirectoryFiles");

    g_lua.instanceFunction(&FileManager::makeDirectory,     &g_fileManager, "g_fileManager.makeDirectory");
    g_lua.instanceFunction(&FileManager::deleteFile,        &g_fileManager, "g_fileManager.deleteFile");
    g_lua.instanceFunction(&FileManager::readFileString,    &g_fileManager, "g_fileManager.readFileString");
    g_lua.instanceFunction(&FileManager::writeFileBuffer,   &g_fileManager, "g_fileManager.writeFileBuffer");
    g_lua.instanceFunction(&FileManager::writeFileString,   &g_fileManager, "g_fileManager.writeFileString");
    g_lua.instanceFunction(&FileManager::openFile,          &g_fileManager, "g_fileManager.openFile");
    g_lua.instanceFunction(&FileManager::appendFile,        &g_fileManager, "g_fileManager.appendFile");
    g_lua.instanceFunction(&FileManager::writeFile,         &g_fileManager, "g_fileManager.writeFile");

    g_lua.staticFunction(std::function<void(const std::string&)>([]
        (const std::string& path)
        {
			if(!g_lua.doString("", g_fileManager.readFileString(path).c_str()))
				LOGE("Unable to load: %s", path.c_str());
        }),                                        "g_fileManager.call");

    g_lua.newMetatable("FileStream");
    g_lua.destructor<FileStream>("FileStream.delete");
    g_lua.sharedFunction(&FileStream::seek,              "FileStream.seek");
    g_lua.sharedFunction(&FileStream::tell,              "FileStream.tell");
    g_lua.sharedFunction(&FileStream::size,              "FileStream.size");
    g_lua.sharedFunction(&FileStream::empty,             "FileStream.empty");

    g_lua.sharedFunction(&FileStream::getFileString,     "FileStream.getFileString");
    g_lua.sharedFunction(&FileStream::get<uint8_t>,      "FileStream.getU8");
    g_lua.sharedFunction(&FileStream::get<uint16_t>,     "FileStream.getU16");
    g_lua.sharedFunction(&FileStream::get<uint32_t>,     "FileStream.getU32");
    g_lua.sharedFunction(&FileStream::get<uint64_t>,     "FileStream.getU64");
    g_lua.sharedFunction(&FileStream::get<int8_t>,       "FileStream.get8");
    g_lua.sharedFunction(&FileStream::get<int16_t>,      "FileStream.get16");
    g_lua.sharedFunction(&FileStream::get<int32_t>,      "FileStream.get32");
    g_lua.sharedFunction(&FileStream::get<int64_t>,      "FileStream.get64");
    g_lua.sharedFunction(&FileStream::get<std::string>,  "FileStream.getString");

    g_lua.sharedFunction(&FileStream::put<uint8_t>,      "FileStream.putU8");
    g_lua.sharedFunction(&FileStream::put<uint16_t>,     "FileStream.putU16");
    g_lua.sharedFunction(&FileStream::put<uint32_t>,     "FileStream.putU32");
    g_lua.sharedFunction(&FileStream::put<uint64_t>,     "FileStream.putU64");
    g_lua.sharedFunction(&FileStream::put<int8_t>,       "FileStream.put8");
    g_lua.sharedFunction(&FileStream::put<int16_t>,      "FileStream.put16");
    g_lua.sharedFunction(&FileStream::put<int32_t>,      "FileStream.put32");
    g_lua.sharedFunction(&FileStream::put<int64_t>,      "FileStream.put64");
    g_lua.sharedFunction(&FileStream::put<std::string>,  "FileStream.putString");

    g_lua.newMetatable("Scheduler");
    g_lua.constructor<Scheduler>("Scheduler.new");
    g_lua.destructor<Scheduler>("Scheduler.delete");
    g_lua.sharedFunction(&Scheduler::addEvent,          "Scheduler.addEvent");
    g_lua.sharedFunction(&Scheduler::scheduleEvent,     "Scheduler.scheduleEvent");
    g_lua.sharedFunction(&Scheduler::stopEvent,         "Scheduler.stopEvent");
    g_lua.sharedFunction(&Scheduler::pause,             "Scheduler.pause");
    g_lua.sharedFunction(&Scheduler::resume,            "Scheduler.resume");
    g_lua.sharedFunction(&Scheduler::isPaused,          "Scheduler.isPaused");

    g_lua.instanceFunction(&Scheduler::addEvent,        g_scheduler, "addEvent");
    g_lua.instanceFunction(&Scheduler::scheduleEvent,   g_scheduler, "scheduleEvent");
    g_lua.instanceFunction(&Scheduler::stopEvent,       g_scheduler, "stopEvent");

    /////////////////////////////////////////////////////////////////////////////////////////////

    g_lua.newMetatable("Mouse");
    g_lua.instanceFunction(&Platform::isMousePressed,        &g_platform, "Mouse.isPressed");
    g_lua.instanceFunction(&Platform::getMousePosition,      &g_platform, "Mouse.getPosition");
    g_lua.instanceFunction(&Platform::getMouseLastPosition,  &g_platform, "Mouse.getLastPosition");
    g_lua.instanceFunction(&Platform::getMouseClickPosition, &g_platform, "Mouse.getMouseClickPosition");

    g_lua.instanceFunction(&Platform::isKeyPressed,          &g_platform, "isKeyPressed");

    g_lua.instanceFunction(&Platform::getFocusedWidget,      &g_platform, "getFocusedWidget");
    g_lua.instanceFunction(&Platform::getGrabbedWidget,      &g_platform, "getGrabbedWidget");
    g_lua.instanceFunction(&Platform::getRootWidget,         &g_platform, "getRootWidget");
    g_lua.instanceFunction(&Platform::exit,                  &g_platform, "closeApplication");
    g_lua.staticFunction(&Platform::openWebsite, "openWebsite");
    g_lua.staticFunction(&Platform::GetIpAddr,   "GetIpAddr");

    g_lua.instanceFunction(&Graphic::setDrawDimension, g_graphic, "setDrawDimension");
    g_lua.instanceFunction(&Graphic::getDrawDimension, g_graphic, "getDrawDimension");
    g_lua.instanceFunction(&Graphic::getFps, g_graphic, "getFps");
    g_lua.staticFunction(&getTicks, "getTicks");

    g_lua.newMetatable("Config");
    g_lua.constructor<Config, const std::string&>("Config.new");
    g_lua.destructor<Config>("Config.delete");
    g_lua.sharedFunction(&Config::load,            "Config.load");
    g_lua.sharedFunction(&Config::save,            "Config.save");
    g_lua.sharedFunction(&Config::set,             "Config.set");
    g_lua.sharedFunction(&Config::get,             "Config.get");
    g_lua.sharedFunction(&Config::erase,           "Config.erase");
    g_lua.sharedFunction(&Config::clear,           "Config.clear");


    g_lua.newMetatable("Json");
    g_lua.staticFunction(std::function<std::string(const json11::Json*, const std::string&)>([]
        (const json11::Json* json, const std::string& field)
        { return stdex::toString((*json)[field]); }),                  "Json.toString");

    g_lua.staticFunction(std::function<int32_t(const json11::Json*, const std::string&)>([]
        (const json11::Json* json, const std::string& field)
        { return (int32_t)stdex::toInteger((*json)[field]); }),        "Json.toInteger");

    g_lua.staticFunction(std::function<bool(const json11::Json*, const std::string&)>([]
        (const json11::Json* json, const std::string& field)
        { return stdex::toBoolean((*json)[field]); }),                 "Json.toBoolean");

    g_lua.staticFunction(std::function<Point(const json11::Json*, const std::string&)>([]
        (const json11::Json* json, const std::string& field)
        { return stdex::toPoint((*json)[field]); }),                   "Json.toPoint");

    g_lua.staticFunction(std::function<Rect(const json11::Json*, const std::string&)>([]
        (const json11::Json* json, const std::string& field)
        { return stdex::toRect((*json)[field]); }),                    "Json.toRect");

    g_lua.staticFunction(std::function<Rect(const json11::Json*, const std::string&)>([]
        (const json11::Json* json, const std::string& field)
        { return stdex::toRectSize((*json)[field]); }),                "Json.toSize");

    g_lua.staticFunction(std::function<Color(const json11::Json*, const std::string&)>([]
        (const json11::Json* json, const std::string& field)
        { return stdex::toColor((*json)[field]); }),                   "Json.toColor");

    g_lua.staticFunction(std::function<Anchor::Edge(const json11::Json*, const std::string&)>([]
        (const json11::Json* json, const std::string& field)
        { return stdex::toEdge((*json)[field]); }),                    "Json.toEdge");

    g_lua.instanceFunction(&Lua::State::newMetatable, &g_lua, "newMetatable");
    lua_register(g_lua.state(), "print", print);
}
