// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLevelHUD.h"
#include "TutorialLevelOverlay.h"

// ATutorialLevelHUD Ŭ������ ������ �Լ�
// ����� �ƹ� �۾��� ���� ����
ATutorialLevelHUD::ATutorialLevelHUD() {

}

// BeginPlay�� Actor�� ���ӿ� ������ �� �� ���� ȣ��Ǵ� �Լ�
void ATutorialLevelHUD::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay ȣ��
	Super::BeginPlay();

	// World ��ü�� �������ų� �̹� �ִٸ� �״�� ���
	// World�� ���� ������ ��� ����Ƽ�� �����ϴ� ��ü
	World = World == nullptr ? GetWorld() : World;

	// World ��ü�� ��ȿ�ϴٸ� PlayerController�� ������
	// PlayerController�� ������� �Է��� ó���ϴ� ��ü
	if (World) {
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
	}

	// PlayerController�� TutorialLevelOverlayClass�� ��ȿ�ϴٸ�
	// ���콺 Ŀ���� ���̰� �ϰ� TutorialLevelOverlay ������ �����Ͽ� ����Ʈ�� �߰�
	if (PlayerController && TutorialLevelOverlayClass) {
		PlayerController->bShowMouseCursor = true; // ���콺 Ŀ�� ���̰� ����
		// TutorialLevelOverlay ���� ����
		TutorialLevelOverlay = CreateWidget<UTutorialLevelOverlay>(PlayerController, TutorialLevelOverlayClass);
		TutorialLevelOverlay->AddToViewport(); // ������ ������ ����Ʈ�� �߰�
	}
}

