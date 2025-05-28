// https://www.youtube.com/watch?v=08-2nT6cul0

#include "MetasoundExecutableOperator.h"
#include "MetasoundPrimitives.h"
#include "MetasoundNodeRegistrationMacro.h"
#include "MetasoundStandardNodesNames.h"
#include "MetasoundFacade.h"
#include "MetasoundParamHelper.h"

#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_MetaSoundFloatAdditionNode"

namespace Metasound {

namespace DeltaMetaSoundFloatAdditionNodeNames {

METASOUND_PARAM(InputAValue, "A", "Input value A.");
METASOUND_PARAM(InputBValue, "B", "Input value B.");

METASOUND_PARAM(OutputValue, "Sum of A and B", "The sum of A and B.");

} // namespace DeltaMetaSoundFloatAdditionNodeNames

class FDeltaMetaSoundFloatAdditionOperator
  : public TExecutableOperator<FDeltaMetaSoundFloatAdditionOperator> {
public:
  FDeltaMetaSoundFloatAdditionOperator(const FFloatReadRef& InAValue, const FFloatReadRef& InBValue)
    : InputA(InAValue)
    , InputB(InBValue)
    , OutputA(FFloatWriteRef::CreateNew(*InputA + *InputB)) {
  }

  static const FVertexInterface& DeclareVertexInterface() {
    using namespace DeltaMetaSoundFloatAdditionNodeNames;

    static const FVertexInterface Interface(
      FInputVertexInterface(
        TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InputAValue)),
        TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InputBValue))),

      FOutputVertexInterface(
        TOutputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutputValue))));

    return Interface;
  }

  static const FNodeClassMetadata& GetNodeInfo() {
    auto CreateNodeClassMetadata = []() -> FNodeClassMetadata {
      FVertexInterface NodeInterface = DeclareVertexInterface();

      FNodeClassMetadata Metadata{
        FNodeClassName{FName(TEXT("UE5")),
                       TEXT("DeltaMetaSoundFloatAdditionNode"),
                       FName(TEXT("Audio"))},
        1,
        0,
        METASOUND_LOCTEXT("DeltaMetaSoundFloatAdditionNodeDisplayName", "FloatAdditionNode"),
        METASOUND_LOCTEXT("DeltaMetaSoundFloatAdditionNodeDesc", "Add two float number"),
        PluginAuthor,
        PluginNodeMissingPrompt,
        NodeInterface,
        {},
        {},
        FNodeDisplayStyle{}};

      return Metadata;
    };

    static const FNodeClassMetadata Metadata = CreateNodeClassMetadata();
    return Metadata;
  }

  virtual FDataReferenceCollection GetInputs() const override {
    using namespace DeltaMetaSoundFloatAdditionNodeNames;

    FDataReferenceCollection InputDataReferences;

    InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputAValue), InputA);
    InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputBValue), InputB);

    return InputDataReferences;
  }

  virtual FDataReferenceCollection GetOutputs() const override {
    using namespace DeltaMetaSoundFloatAdditionNodeNames;

    FDataReferenceCollection OutputDataReferences;

    OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputValue), OutputA);

    return OutputDataReferences;
  }

  static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams,
                                              FBuildErrorArray&            OutErrors) {
    using namespace DeltaMetaSoundFloatAdditionNodeNames;

    const Metasound::FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
    const Metasound::FInputVertexInterface&    InputInterface =
      DeclareVertexInterface().GetInputInterface();

    TDataReadReference<float> InputA =
      InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(
        InputInterface,
        METASOUND_GET_PARAM_NAME(InputAValue),
        InParams.OperatorSettings);

    TDataReadReference<float> InputB =
      InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(
        InputInterface,
        METASOUND_GET_PARAM_NAME(InputBValue),
        InParams.OperatorSettings);

    return MakeUnique<FDeltaMetaSoundFloatAdditionOperator>(InputA, InputB);
  }

  void Execute() {
    *OutputA = *InputA + *InputB;
  }

private:
  FFloatReadRef  InputA;
  FFloatReadRef  InputB;
  FFloatWriteRef OutputA;
};

class FDeltaMetaSoundFloatAdditionNode : public FNodeFacade {
public:
  FDeltaMetaSoundFloatAdditionNode(const FNodeInitData& InitData)
    : FNodeFacade(InitData.InstanceName,
                  InitData.InstanceID,
                  TFacadeOperatorClass<FDeltaMetaSoundFloatAdditionOperator>()) {
  }
};

METASOUND_REGISTER_NODE(FDeltaMetaSoundFloatAdditionNode);

} // namespace Metasound

#undef LOCTEXT_NAMESPACE