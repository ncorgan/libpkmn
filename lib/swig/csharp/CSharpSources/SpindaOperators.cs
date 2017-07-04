/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

namespace PKMN {

    public partial class SpindaCoords {

        public static SpindaCoords operator +(
            SpindaCoords coords1,
            SpindaCoords coords2
        ) {
            return new SpindaCoords(
                           coords1.X + coords2.X,
                           coords1.Y + coords2.Y
                       );
        }

    }

    public partial class SpindaSpots {

        public static SpindaSpots operator +(
            SpindaSpots spots1,
            SpindaSpots spots2
        ) {
            return new SpindaSpots(
                           spots1.LeftEar + spots2.LeftEar,
                           spots1.RightEar + spots2.RightEar,
                           spots1.LeftFace + spots2.LeftFace,
                           spots1.RightFace + spots2.RightFace
                       );
        }

        public static SpindaSpots operator +(
            SpindaSpots spots,
            SpindaCoords coords
        ) {
            return new SpindaSpots(
                           spots.LeftEar + coords,
                           spots.RightEar + coords,
                           spots.LeftFace + coords,
                           spots.RightFace + coords
                       );
        }

    }

}
