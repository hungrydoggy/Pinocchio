﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace Primitives3D
{
    class PlanePrimitive : GeometricPrimitive
    {
        
        /// <summary>
        /// Constructs a new cube primitive, using default settings.
        /// </summary>
        public PlanePrimitive(GraphicsDevice graphicsDevice)
            : this(graphicsDevice, new Vector3(1.0f), Vector3.Zero)
        {
        }


        public PlanePrimitive(GraphicsDevice graphicsDevice, Vector3 size)
            : this(graphicsDevice, size, Vector3.Zero)
        {
        }

        /// <summary>
        /// Constructs a new cube primitive, with the specified size.
        /// </summary>
        public PlanePrimitive(GraphicsDevice graphicsDevice, Vector3 size, Vector3 center)
        {
            // A cube has six faces, each one pointing in a different direction.
            Vector3 normal = new Vector3(0, 0, 1);
            Vector3[] vertices =
            {
                new Vector3(-0.5f, 0.5f, 0.0f),
                new Vector3(-0.5f, -0.5f, 0.0f),
                new Vector3(0.5f, -0.5f, 0.0f),
                new Vector3(0.5f, 0.5f, 0.0f),
            };
            
            // Six indices (two triangles) per face.
            AddIndex(0);
            AddIndex(1);
            AddIndex(2);
            AddIndex(0);
            AddIndex(2);
            AddIndex(3);

            // Create each face in turn.
            foreach (Vector3 vertex in vertices)
            {
                AddVertex(vertex, normal);
            }

            InitializePrimitive(graphicsDevice);
        }
    }
}
