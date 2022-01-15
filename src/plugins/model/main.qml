import QtQuick 2.0

import QtQuick 2.1 as QQ2
import QtQuick.Scene3D 2.0
import Qt3D.Core 2.15
import Qt3D.Render 2.15
import Qt3D.Input 2.0
import Qt3D.Extras 2.15
import vangers_utils 1.0

Item {
    property DrawData vertexData: vertexBufferData
    property DrawData indexData: indexBufferData

    Scene3D {
        id: scene3d
        anchors.fill: parent
        anchors.margins: 10
        focus: true
        aspects: ["input", "logic"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio


        Entity {
            Camera {
                id: camera
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 45
                nearPlane : 0.1
                farPlane : 1000.0
                position: Qt.vector3d( 0.0, 0.0, 40.0 )
                upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
            }

            OrbitCameraController {
                camera: camera ;
                lookSpeed: -200;
                linearSpeed: -50;

            }

            components: [
                RenderSettings {
                    id: renderSettings
                    activeFrameGraph: ForwardRenderer {
                        camera: camera
                        clearColor: "transparent"
                        showDebugOverlay: false
                    }
                    pickingSettings.faceOrientationPickingMode: PickingSettings.FrontAndBackFace
                },
                InputSettings { }
            ]

            PhongMaterial {
                id: materialPhong
                ambient: "lightgray"
            }

            CylinderMesh {
                id: cylinderMesh
                radius: 15
                length: .03
                rings: 100
                slices: 20
            }

            Transform {
                id: torusTransform
                translation: Qt.vector3d(0, -7, 0)
//                scale3D: Qt.vector3d(1.5, 1, 0.5)
//                rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
            }

            Entity {
                id: torusEntity
                components: [ cylinderMesh, materialPhong, torusTransform ]
            }

            PerVertexColorMaterial {
                id: material
            }

            Transform {
                id: transform
            }

            Transform {
                id: transform2
                translation: Qt.vector3d(1, 1, 0)
            }

            VangersMaterial {
                id: vangersMaterial
            }



            WireframeMaterial {
                id: wireframeMaterial
                effect: WireframeEffect {}
                ambient: Qt.rgba( 0.2, 0.0, 0.0, 1.0 )
                diffuse: Qt.rgba( 0.8, 0.0, 0.0, 1.0 )
                lineWidth: .7

//                QQ2.SequentialAnimation {
//                    loops: QQ2.Animation.Infinite
//                    running: true

//                    QQ2.NumberAnimation {
//                        target: wireframeMaterial;
//                        property: "lineWidth";
//                        duration: 1000;
//                        from: 0.8
//                        to: 1.8
//                    }

//                    QQ2.NumberAnimation {
//                        target: wireframeMaterial;
//                        property: "lineWidth";
//                        duration: 1000;
//                        from: 1.8
//                        to: 0.8
//                    }

//                    QQ2.PauseAnimation { duration: 1500 }
//                }
            }

            Entity {
                id: geometryEntity

                components: [geometryRenderer, wireframeMaterial]


                GeometryRenderer {
                    id: geometryRenderer
                    primitiveType: GeometryRenderer.Triangles
//                    instanceCount: 2



                    geometry: Geometry {

                      Attribute {
                        attributeType: Attribute.VertexAttribute
                        vertexBaseType: Attribute.Float
                        vertexSize: 3
                        count: 4
                        byteOffset: 0
                        byteStride: 9 * 4
                        name: defaultPositionAttributeName
//                        buffer: vertexBuffer
                        buffer: vertexBufferData.buffer
                      }

                      Attribute {
                        attributeType: Attribute.VertexAttribute
                        vertexBaseType: Attribute.Float
                        vertexSize: 3
                        count: vertexBufferData.count
                        byteOffset: 6 * 4
                        byteStride: 9 * 4
                        name: defaultColorAttributeName
                        buffer: vertexBufferData.buffer
                      }

                      Attribute {
                        attributeType: Attribute.VertexAttribute
                        vertexBaseType: Attribute.Float
                        vertexSize: 3
                        count: vertexBufferData.count
                        byteOffset: 3 * 4
                        byteStride: 9 * 4
                        name: defaultNormalAttributeName
                        buffer: vertexBufferData.buffer
                      }

                      Attribute {
                        attributeType: Attribute.IndexAttribute
                        vertexBaseType: Attribute.UnsignedInt
                        vertexSize: 1
                        count: indexBufferData.count
                        byteOffset: 0
                        byteStride: 1 * 4 // 1 index * sizeof(Uint32)
                        buffer: indexBufferData.buffer
                      }

                    }

                    DrawData {
                        id: vertexBufferData;
                    }

                    DrawData {
                        id: indexBufferData;
                    }


//                    Buffer {
//                      id: indexBuffer
//                      objectName: "indexBuffer"
//                      type: Buffer.IndexBuffer
//                      data: new Uint32Array([
//                        0, 1, 2,
//                        0, 2, 3,
//                      ])
//                    }

//                    Buffer {
//                      id: vertexBuffer
//                      objectName: "vertexBuffer"
//                      type: Buffer.VertexBuffer
//                      data: new Float32Array([
//                        -2.0, -3.0, -2.0,
//                        2.0, -2.0, -2.0,
//                        2.0,  3.0, -2.0,
//                        -2.0,  2.0, -2.0,
//                      ])
//                    }

                }
            }
        }
    }
}

