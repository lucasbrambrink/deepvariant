/*
 * Copyright 2023 Google LLC.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LEARNING_GENOMICS_DEEPVARIANT_MAKE_EXAMPLES_NATIVE_H_
#define LEARNING_GENOMICS_DEEPVARIANT_MAKE_EXAMPLES_NATIVE_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "deepvariant/pileup_image_native.h"
#include "deepvariant/protos/deepvariant.pb.h"
#include "absl/container/flat_hash_map.h"
#include "third_party/nucleus/io/reference.h"
#include "third_party/nucleus/io/tfrecord_writer.h"
#include "third_party/nucleus/protos/variants.pb.h"

namespace learning {
namespace genomics {
namespace deepvariant {

// This enum matches the Python enum in
// dv_utils_using_clif.py
enum class EncodedVariantType {
  kUnknown = 0,
  kSnp = 1,
  kIndel = 2,
};

// Different ways alt aligned reads can be expressed.
// This enum matches flag values in make_example_options.
// It helps to avoid string comparison in performance critical parts.
enum AltAlignedPileup {
  kNone = 0,
  kBaseChannels = 1,
  kDiffChannels = 2,
  kRows = 3,
};

struct Sample {
  Sample() = default;
  explicit Sample(const SampleOptions& options) : sample_options(options){};

  SampleOptions sample_options;
  std::unique_ptr<nucleus::TFRecordWriter> writer;
};

// Cache of reads. Reads are passed from Python wrapped in ConstProtoPtr.
// InMemoryReader serves as a container of Read pointers passed from Python.
// Query returns a vector of pointers to avoid copying.
class InMemoryReader {
 public:
  explicit InMemoryReader(
      const std::vector<nucleus::ConstProtoPtr<nucleus::genomics::v1::Read>>&
          reads);

  // Returns reads overlapping the range.
  std::vector<const nucleus::genomics::v1::Read*> Query(
      const nucleus::genomics::v1::Range& range) const;

 private:
  const std::vector<nucleus::ConstProtoPtr<nucleus::genomics::v1::Read>>&
      reads_cache_;
};

// Generates TensorFlow examples from candidates and reads.
class ExamplesGenerator {
 public:
  explicit ExamplesGenerator(const MakeExamplesOptions& options,
                             bool test_mode = false);

  ~ExamplesGenerator();

  // Encodes candidates into TensorFlow examples containing pileup images
  // and writes them to TFRecord.
  void WriteExamplesInRegion(
      const std::vector<nucleus::ConstProtoPtr<DeepVariantCall>>& candidates,
      const std::vector<
          std::vector<nucleus::ConstProtoPtr<nucleus::genomics::v1::Read>>>&
          reads_per_sample,
      const std::vector<int>& sample_order,
      // std::string has to be used here instead of absl::string view due to the
      // PyClif restrictions.
      const std::string& role);

 private:
  friend class ExamplesGeneratorPeer;

  // Generates all pairs of alt alleles and ref.
  std::vector<std::vector<std::string>> AltAlleleCombinations(
      const nucleus::genomics::v1::Variant& variant) const;

  // Creates haplotype by concatenating alt bases and reference from both sides.
  // Haplotype length should be equal pileup image width.
  // ref_start_out and ref_end_out are haplotype's start and end in reference
  // coordinates.
  std::string CreateHaplotype(const nucleus::genomics::v1::Variant& variant,
                              const std::string& alt, int64_t* ref_start_out,
                              int64_t* ref_end_out) const;

  // Encodes a variant into a pileup example.
  std::string EncodeExample(
      const std::vector<std::unique_ptr<ImageRow>>& image,
      const std::vector<std::vector<std::unique_ptr<ImageRow>>>& alt_image,
      const nucleus::genomics::v1::Variant& variant,
      const std::vector<std::string>& alt_combination) const;

  // Generates one or more examples from a given candidate. Example(s) are
  // written to TFRecord.
  void CreateAndWriteExamplesForCandidate(
      const DeepVariantCall& candidate, const Sample& sample,
      const std::vector<int>& sample_order,
      const std::vector<InMemoryReader>& readers);

  void CreateAltAlignedImages(
      const DeepVariantCall& candidate,
      const std::vector<std::string>& alt_combination,
      const std::vector<nucleus::genomics::v1::Read>& trimmed_reads,
      int sample_order,
      const nucleus::genomics::v1::Range& region,
      std::vector<std::vector<std::unique_ptr<ImageRow>>>& alt_images);

  // Returns true if the variant needs alt alignment.
  bool NeedAltAlignment(const nucleus::genomics::v1::Variant& variant) const;

  // Helper methods to extract reference bases for a given range.
  std::string GetReferenceBasesForPileup(
      const nucleus::genomics::v1::Variant& variant) const;
  std::string GetReferenceBases(
      const nucleus::genomics::v1::Range& region) const;
  // Make examples config.
  const MakeExamplesOptions options_;

  std::unique_ptr<nucleus::GenomeReference> ref_reader_;

  // Half width of pileup image.
  int half_width_;

  // PileupImageEncoderNative instance.
  PileupImageEncoderNative pileup_image_;

  // Samples keyed by role.
  absl::flat_hash_map<std::string, Sample> samples_;

  // Alt aligned pileup option.
  AltAlignedPileup alt_aligned_pileup_;
};

// Helper class to allow unit testing of some private methods.
class ExamplesGeneratorPeer {
 public:
  // Calls private AltAlleleCombinations.
  static std::vector<std::vector<std::string>> CallAltAlleleCombinations(
      const ExamplesGenerator& generator,
      const nucleus::genomics::v1::Variant& variant) {
    return generator.AltAlleleCombinations(variant);
  }

  static std::string CallCreateHaplotype(
      const ExamplesGenerator& generator,
      const nucleus::genomics::v1::Variant& variant, const std::string& alt,
      int64_t* ref_start_out, int64_t* ref_end_out) {
    return generator.CreateHaplotype(variant, alt, ref_start_out, ref_end_out);
  }

  static void SetRefReader(
      ExamplesGenerator& generator,
      std::unique_ptr<nucleus::GenomeReference> ref_reader) {
    generator.ref_reader_ = std::move(ref_reader);
  }

  static bool NeedAltAlignment(const ExamplesGenerator& generator,
                               const nucleus::genomics::v1::Variant& variant) {
    return generator.NeedAltAlignment(variant);
  }
};

nucleus::genomics::v1::Range MakeRange(const std::string& ref_name,
                                       int64_t start, int64_t end);

std::string GetExamplesFilename(const MakeExamplesOptions& options,
                                const Sample& sample);

void FillPileupArray(
    const std::vector<std::unique_ptr<ImageRow>>& image,
    const std::vector<std::vector<std::unique_ptr<ImageRow>>>& alt_image,
    AltAlignedPileup alt_aligned_representation,
    std::vector<unsigned char>* pileup_array);

}  // namespace deepvariant
}  // namespace genomics
}  // namespace learning

#endif  // LEARNING_GENOMICS_DEEPVARIANT_MAKE_EXAMPLES_NATIVE_H_
