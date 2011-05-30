//
//  Malvar_He_Cutler_Bayer_DemosaicingPlugInLoader.h
//  Malvar-He-Cutler Bayer Demosaicing
//
//	Original GPU implementation by Morgan McGuire in mcguire09bayer cf:
//
//	BibTex
//
//	@article{mcguire09bayer,
//		author = {Morgan Mc{G}uire},
//		title = {Efficient, High-Quality Bayer Demosaic Filtering on GPUs},
//		journal = {Submitted to Journal of Graphics Tools},
//		year = {2009},
//		publisher = {AK Peters},
//		address = {Wellesley, MA}
//	}
//
//  Implemented as Core Image Filter by Ole Kristensen 2011
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>


@interface Malvar_He_Cutler_Bayer_DemosaicingFilter : CIFilter
{
    CIImage      *inputImage;
    CIVector     *inputFirstRedPixel;
}

@end
