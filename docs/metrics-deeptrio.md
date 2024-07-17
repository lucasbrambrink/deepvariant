# DeepTrio runtime and accuracy metrics for all release models

## WGS (Illumina)

### Runtime

Runtime is on HG002/HG003/HG004 (all chromosomes).

Stage                            | Wall time (minutes)
-------------------------------- | -----------------
make_examples                    | ~439m
call_variants for HG002          | ~351m
call_variants for HG003          | ~355m
call_variants for HG004          | ~361m
postprocess_variants (parallel)  | ~61m
total                            | ~1567m = ~26.12 hours

### Accuracy

We report hap.py results on HG002/HG003/HG004 trio (chr20, using NIST v4.2.1
truth), which was held out while training.

#### HG002:

| Type  | TRUTH.TP | TRUTH.FN | QUERY.FP | METRIC.Recall | METRIC.Precision | METRIC.F1_Score |
| ----- | -------- | -------- | -------- | ------------- | ---------------- | --------------- |
| INDEL | 11208    | 48       | 13       | 0.995736      | 0.998884         | 0.997308        |
| SNP   | 71087    | 246      | 42       | 0.996551      | 0.99941          | 0.997979        |

#### HG003:

| Type  | TRUTH.TP | TRUTH.FN | QUERY.FP | METRIC.Recall | METRIC.Precision | METRIC.F1_Score |
| ----- | -------- | -------- | -------- | ------------- | ---------------- | --------------- |
| INDEL | 10584    | 44       | 20       | 0.99586       | 0.998192         | 0.997024        |
| SNP   | 69975    | 191      | 55       | 0.997278      | 0.999215         | 0.998246        |

#### HG004:

| Type  | TRUTH.TP | TRUTH.FN | QUERY.FP | METRIC.Recall | METRIC.Precision | METRIC.F1_Score |
| ----- | -------- | -------- | -------- | ------------- | ---------------- | --------------- |
| INDEL | 10945    | 55       | 27       | 0.995         | 0.997643         | 0.99632         |
| SNP   | 71446    | 213      | 52       | 0.997028      | 0.999273         | 0.998149        |

* See VCF stats report (for all chromosomes)
  - [HG002](https://storage.googleapis.com/deepvariant/visual_reports/DeepTrio/1.6.1/WGS/HG002.output.visual_report.html)
  - [HG003](https://storage.googleapis.com/deepvariant/visual_reports/DeepTrio/1.6.1/WGS/HG003.output.visual_report.html)
  - [HG004](https://storage.googleapis.com/deepvariant/visual_reports/DeepTrio/1.6.1/WGS/HG004.output.visual_report.html)

## PacBio (HiFi)

In v1.6.1, we introduced read haplotagging in DeepTrio PacBio. You no longer
need to run DeepVariant->WhatsHap->DeepTrio, and can just run DeepTrio once.

### Runtime

Runtime is on HG002/HG003/HG004 (all chromosomes).

Stage                            | Wall time (minutes)
-------------------------------- | -------------------
make_examples                    | ~51+1078m
call_variants for HG002          | ~364m
call_variants for HG003          | ~365m
call_variants for HG004          | ~372m
postprocess_variants (parallel)  | ~88m
total                            | ~2318m = ~38.63 hours

* See VCF stats report (for all chromosomes)
  - [HG002](https://storage.googleapis.com/deepvariant/visual_reports/DeepTrio/1.6.1/PACBIO/HG002.output.visual_report.html)
  - [HG003](https://storage.googleapis.com/deepvariant/visual_reports/DeepTrio/1.6.1/PACBIO/HG003.output.visual_report.html)
  - [HG004](https://storage.googleapis.com/deepvariant/visual_reports/DeepTrio/1.6.1/PACBIO/HG004.output.visual_report.html)

### Accuracy

We report hap.py results on HG002/HG003/HG004 trio (chr20, using NIST v4.2.1
truth), which was held out while training.

#### HG002:

| Type  | TRUTH.TP | TRUTH.FN | QUERY.FP | METRIC.Recall | METRIC.Precision | METRIC.F1_Score |
| ----- | -------- | -------- | -------- | ------------- | ---------------- | --------------- |
| INDEL | 11215    | 41       | 85       | 0.996357      | 0.992777         | 0.994564        |
| SNP   | 71303    | 30       | 20       | 0.999579      | 0.99972          | 0.99965         |

#### HG003:

| Type  | TRUTH.TP | TRUTH.FN | QUERY.FP | METRIC.Recall | METRIC.Precision | METRIC.F1_Score |
| ----- | -------- | -------- | -------- | ------------- | ---------------- | --------------- |
| INDEL | 10575    | 53       | 78       | 0.995013      | 0.993            | 0.994006        |
| SNP   | 70145    | 21       | 35       | 0.999701      | 0.999502         | 0.999601        |

#### HG004:

| Type  | TRUTH.TP | TRUTH.FN | QUERY.FP | METRIC.Recall | METRIC.Precision | METRIC.F1_Score |
| ----- | -------- | -------- | -------- | ------------- | ---------------- | --------------- |
| INDEL | 10957    | 43       | 60       | 0.996091      | 0.994796         | 0.995443        |
| SNP   | 71621    | 38       | 28       | 0.99947       | 0.99961          | 0.99954         |

## Whole Exome Sequencing (Illumina)

### Runtime

Runtime is on HG002/HG003/HG004 (all chromosomes).

Stage                            | Wall time (minutes)
-------------------------------- | --------------
make_examples                    | ~17m
call_variants for HG002          | ~5m
call_variants for HG003          | ~5m
call_variants for HG004          | ~5m
postprocess_variants (parallel)  | ~1m
total                            | ~33m

### Accuracy

We report hap.py results on HG002/HG003/HG004 trio (chr20, using NIST v4.2.1
truth), which was held out while training.

#### HG002:

| Type  | TRUTH.TP | TRUTH.FN | QUERY.FP | METRIC.Recall | METRIC.Precision | METRIC.F1_Score |
| ----- | -------- | -------- | -------- | ------------- | ---------------- | --------------- |
| INDEL | 34       | 0        | 0        | 1.0           | 1.0              | 1.0             |
| SNP   | 670      | 2        | 0        | 0.997024      | 1.0              | 0.99851         |

#### HG003:

| Type  | TRUTH.TP | TRUTH.FN | QUERY.FP | METRIC.Recall | METRIC.Precision | METRIC.F1_Score |
| ----- | -------- | -------- | -------- | ------------- | ---------------- | --------------- |
| INDEL | 29       | 0        | 0        | 1.0           | 1.0              | 1.0             |
| SNP   | 683      | 2        | 0        | 0.99708       | 1.0              | 0.998538        |


#### HG004:

| Type  | TRUTH.TP | TRUTH.FN | QUERY.FP | METRIC.Recall | METRIC.Precision | METRIC.F1_Score |
| ----- | -------- | -------- | -------- | ------------- | ---------------- | --------------- |
| INDEL | 32       | 1        | 1        | 0.969697      | 0.969697         | 0.969697        |
| SNP   | 677      | 2        | 0        | 0.997054      | 1.0              | 0.998525        |

* See VCF stats report (for all chromosomes)
  - [HG002](https://storage.googleapis.com/deepvariant/visual_reports/DeepTrio/1.6.1/WES/HG002.output.visual_report.html)
  - [HG003](https://storage.googleapis.com/deepvariant/visual_reports/DeepTrio/1.6.1/WES/HG003.output.visual_report.html)
  - [HG004](https://storage.googleapis.com/deepvariant/visual_reports/DeepTrio/1.6.1/WES/HG004.output.visual_report.html)

## How to reproduce the metrics on this page

For simplicity and consistency, we report runtime with a
[CPU instance with 64 CPUs](deepvariant-details.md#command-for-a-cpu-only-machine-on-google-cloud-platform)
For bigger datasets (WGS and PACBIO), we used bigger disk size (900G).
This is NOT the fastest or cheapest configuration.

Use `gcloud compute ssh` to log in to the newly created instance.

Download and run any of the following case study scripts:

```
curl -O https://raw.githubusercontent.com/google/deepvariant/r1.6.1/scripts/inference_deeptrio.sh

# WGS
bash inference_deeptrio.sh --model_preset WGS

# WES
bash inference_deeptrio.sh --model_preset WES

# PacBio
bash inference_deeptrio.sh --model_preset PACBIO

```

Runtime metrics are taken from the resulting log after each stage of
DeepTrio. The runtime numbers reported above are the average of 5 runs each.
The accuracy metrics come from the hap.py summary.csv output file.
The runs are deterministic so all 5 runs produced the same output.

[CPU instance with 64 CPUs]: deepvariant-details.md#command-for-a-cpu-only-machine-on-google-cloud-platform
