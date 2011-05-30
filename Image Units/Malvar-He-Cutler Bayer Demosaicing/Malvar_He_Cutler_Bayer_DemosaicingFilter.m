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

#import "Malvar_He_Cutler_Bayer_DemosaicingFilter.h"
#import <Foundation/Foundation.h>
#import <ApplicationServices/ApplicationServices.h>

@implementation Malvar_He_Cutler_Bayer_DemosaicingFilter

static CIKernel *_Malvar_He_Cutler_Bayer_DemosaicingFilterKernel = nil;

- (id)init
{
    if(_Malvar_He_Cutler_Bayer_DemosaicingFilterKernel == nil)
    {
		NSBundle    *bundle = [NSBundle bundleForClass:NSClassFromString(@"Malvar_He_Cutler_Bayer_DemosaicingFilter")];
		NSStringEncoding encoding = NSUTF8StringEncoding;
		NSError     *error = nil;
		NSString    *code = [NSString stringWithContentsOfFile:[bundle pathForResource:@"Malvar_He_Cutler_Bayer_DemosaicingFilterKernel" ofType:@"cikernel"] encoding:encoding error:&error];
		NSArray     *kernels = [CIKernel kernelsWithString:code];
		
		_Malvar_He_Cutler_Bayer_DemosaicingFilterKernel = [[kernels objectAtIndex:0] retain];
    }
    return [super init];
}


- (CGRect)regionOf: (int)sampler  destRect: (CGRect)rect
{
    return CGRectInset(rect,-2.0,-2.0);
}


- (NSDictionary *)customAttributes
{
    return [NSDictionary dictionaryWithObjectsAndKeys:
			
			[NSDictionary dictionaryWithObjectsAndKeys:
			 [CIVector vectorWithX:0 Y:1],       kCIAttributeDefault,
			 kCIAttributeTypePosition,           kCIAttributeType,
			 nil],                               @"inputFirstRedPixel",
			
			nil];
}

- (CIImage *)outputImage
{
	CIVector * firstRedPixel;
    CISampler *src;
	
	src = [CISampler samplerWithImage:inputImage];
	firstRedPixel = [CIVector
								vectorWithX:fmin(fmax(0.0, [inputFirstRedPixel X]), 1.0)
								Y:fmin(fmax(0.0, [inputFirstRedPixel Y]), 1.0)
								];
	
	[_Malvar_He_Cutler_Bayer_DemosaicingFilterKernel setROISelector:@selector(regionOf:destRect:)];
	
	return [self apply:_Malvar_He_Cutler_Bayer_DemosaicingFilterKernel, 
			src, firstRedPixel, kCIApplyOptionDefinition, [src definition], nil];
}

@end
